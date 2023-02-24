#include "Camera.h"
#include "ToonGame.h"
#include "ToonUtils.h"
#include "ToonFollowCamera.h"
#include "ToonMinimapCamera.h"
#include "ToonMapCamera.h"
#include "ToonObserverCamera.h"
#include "ToonRaycastCallback.h"
#include "PaintBallClass.h"
#include "ToonEventListener.h"
#include "InputManager.h"

using namespace NCL;
using namespace CSC8503;


ToonGame::ToonGame(GameTechRenderer* renderer, bool offline) : renderer(renderer), offline(offline)
{
	world = new ToonGameWorld();
	renderer->SetWorld(world);

	levelManager = new ToonLevelManager(world);
	world->AddEventListener(new ToonEventListener(&world->GetPhysicsWorld(), world, levelManager));
	baseWeapon = new PaintBallClass(world, levelManager, 15, 500, 0.5f, 1.0f, 5);
	tieTeam = new Team("Draw", Vector3(1, 1, 1), 0);

	StartGame();
}

ToonGame::~ToonGame()
{
	delete world;
	delete baseWeapon;
	delete levelManager;
	delete playerControl;
	delete tieTeam;
}

void ToonGame::StartGame() {
	gameTime = 150.0f;
	winner = nullptr;
	allPlayers.clear();
	if (offline) {
		levelManager->ResetLevel();
		world->SetNetworkStatus(NetworkingStatus::Offline);
		player = levelManager->AddPlayerToWorld(Vector3(20, 5, 0), world->GetTeamLeastPlayers());
		allPlayers.emplace(player);
		playerControl = new PlayerControl();
		player->SetWeapon(baseWeapon);
		world->SetMainCamera(new ToonFollowCamera(world, player));
		world->SetMinimapCamera(new ToonMinimapCamera(*player));
	}
	else {
		levelManager->ResetLevel();
		world->SetMainCamera(new ToonObserverCamera());
	}
	world->SetMapCamera(new ToonMapCamera());
	accumulator = 0.0f;
}

void ToonGame::UpdateGame(float dt){
	if (CheckDebugKeys()) return;

	world->UpdateWorld(dt);
	UpdateCameras(dt, 1);

	if (offline) {
		InputManager::GetInstance().GetInputs()[1]->UpdateGameControls(playerControl, world->GetMainCamera());
		if (player) {
			player->MovementUpdate(dt, playerControl);
			player->WeaponUpdate(dt, playerControl);
		}
	}
	// This next line is an abomination and should be refactored by Ryan
	else if (player) {
		player->SetAiming(playerControl->aiming);
	}

	UpdateAnimations(dt);

	UpdatePhysics(dt);

	UpdateTime(dt);
	ShowUI(gameTime);

	renderer->Render();
	Debug::UpdateRenderables(dt);
}

PushdownState::PushdownResult ToonGame::DidSelectCancelButton()
{
	return PushdownState::Pop;
}

PushdownState::PushdownResult ToonGame::DidSelectOkButton()
{
	m_ShouldQuitGame = true;
	return PushdownState::Pop;
}

PushdownState::PushdownResult ToonGame::OnUpdate(float dt, PushdownState** newState)
{
	if (m_ShouldQuitGame) { return PushdownResult::Pop; }
	if (InputManager::GetInstance().GetInputs()[1]->IsBack() || closeGame)
	{
		*newState = GetToonConfirmationScreen();
		return PushdownResult::Push;
	}
		//return PushdownResult::Pop;
	if (dt > 0.1f)
	{
		std::cout << "Skipping large time delta" << std::endl;
		return PushdownResult::NoChange; //must have hit a breakpoint or something to have a 1 second frame time!
	}
	UpdateGame(dt);

	return PushdownResult::NoChange;
}

void ToonGame::OnAwake()
{
	Window::GetWindow()->ShowOSPointer(false);
	Window::GetWindow()->LockMouseToWindow(true);
}

bool ToonGame::CheckDebugKeys() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8) && offline) {
		StartGame();
		return true;
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9) && (offline || world->GetNetworkStatus() == NetworkingStatus::Server)) {
		gameTime = min(gameTime, 5.0f);
	}
}

void ToonGame::UpdateCameras(float dt, int localPlayer) {
	world->GetMainCamera()->UpdateCamera(dt, InputManager::GetInstance().GetInputs()[localPlayer]);
	if (world->GetMinimapCamera())
		world->GetMinimapCamera()->UpdateCamera(dt, InputManager::GetInstance().GetInputs()[localPlayer]);
}

void ToonGame::UpdatePhysics(float dt) {
	accumulator += dt;
	while (accumulator >= timeStep)
	{
		world->GetPhysicsWorld().update(reactphysics3d::decimal(timeStep));
		accumulator -= timeStep;
		world->DeleteMarkedObjects();
	}
	world->interpolationFactor = float(accumulator / timeStep);
}

void ToonGame::UpdateAnimations(float dt) {
	for (auto& player : allPlayers) {
		player->AnimationUpdate(dt);
	}
}

void ToonGame::UpdateTime(float dt) {
	gameTime -= dt;
	if (gameTime <= 0) {
		if (winner == nullptr && offline == true) {
			winner = DetermineWinner(renderer->GetTeamScores());
		}
		if (gameTime <= -5.0f && offline == true)
			StartGame();
	}
}

void ToonGame::ShowUI(float time) {
	std::string output = "";
	if (time < 0) {
		output += '-';
		time *= -1;
	}
	int minutes = (int)time / 60;
	int seconds = (int)time % 60;
	output += to_string(minutes);
	output += ':';
	if (seconds < 10)
		output += "0";
	output += to_string(seconds);
	Debug::Print(output, NCL::Maths::Vector2(47.5f, 5.0f));

	Debug::Print("[]", Vector2(48.5f, 50.0f), Debug::RED);	//TODO: Hardcoded for now. To be changed later.

	if (winner != nullptr)
		Debug::Print("WINNER:" + winner->GetTeamName(), Vector2(29.5f, 15), winner->GetTeamColour()); //TODO: Hardcoded for now. To be changed later.
}

Team* ToonGame::DetermineWinner(std::map<int, float> teamScores) {
	int currentWinner = 0;
	float winningScore = 0;
	bool tie = false;
	for (auto& [ID, score] : teamScores) {
		if (score > winningScore) {
			winningScore = score;
			currentWinner = ID;
			tie = false;
		}
		else if (score == winningScore)
			tie = true;
	}
	if (tie == true) return tieTeam;
	else return world->GetTeams().find(currentWinner)->second;
}

ToonConfirmationScreen* NCL::CSC8503::ToonGame::GetToonConfirmationScreen()
{
	if (!m_ToonConfirmationScreen)
	{
		m_ToonConfirmationScreen = new ToonConfirmationScreen(Coordinates(Vector2(30, 20), Vector2(50, 20)), m_WindowSize, renderer, "Are you sure, you want to quit the game?");
		m_ToonConfirmationScreen->delegate = this;
	}
	return m_ToonConfirmationScreen;
}
