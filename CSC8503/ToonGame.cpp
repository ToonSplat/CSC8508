#include "Camera.h"
#include "ToonGame.h"
#include "ToonUtils.h"
#include "ToonFollowCamera.h"
#include "ToonMinimapCamera.h"
#include "ToonMapCamera.h"
#include "ToonRaycastCallback.h"
#include "PaintBallClass.h"
#include "ToonEventListener.h"

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
		world->SetMainCamera(new Camera());
	}
	world->SetMapCamera(new ToonMapCamera());
	accumulator = 0.0f;
}

void ToonGame::UpdateGame(float dt){

#pragma region To Be Changed
	Vector2 screenSize = Window::GetWindow()->GetScreenSize();
	Debug::Print("[]", Vector2(48.5f, 50.0f), Debug::RED);	//TODO: Hardcoded for now. To be changed later.
#pragma endregion
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8) && offline) {
		StartGame();
		return;
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9) && (offline || world->GetNetworkStatus() == NetworkingStatus::Server)) {
		gameTime = min(gameTime, 5.0f);
	}
	world->GetMainCamera()->UpdateCamera(dt);
	if (world->GetMinimapCamera())
		world->GetMinimapCamera()->UpdateCamera(dt);
	world->UpdateWorld(dt);

	if (offline) {
		UpdateControls(playerControl);
		if (player) {
			player->MovementUpdate(dt, playerControl);
			player->WeaponUpdate(dt, playerControl);
		}
	}
	// This next line is an abomination and should be refactored by Ryan
	else if (player) {
		player->SetAiming(playerControl->aiming);
	}

	for (auto& player : allPlayers) {
		player->AnimationUpdate(dt);
	}

	accumulator += dt;
	while (accumulator >= timeStep)
	{
		world->GetPhysicsWorld().update(reactphysics3d::decimal(timeStep));
		accumulator -= timeStep;
		world->DeleteMarkedObjects();
	}
	world->interpolationFactor = float(accumulator / timeStep);

	gameTime -= dt;
	ShowTime(gameTime);
	if (gameTime <= 0) {
		if (winner == nullptr && offline == true) {
			winner = DetermineWinner(renderer->GetTeamScores());
		}
		if(winner != nullptr)
			Debug::Print("WINNER: " + winner->GetTeamName(), Vector2(0, 15), winner->GetTeamColour());
		if (gameTime <= -5.0f && offline == true)
			StartGame();
	}
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

void ToonGame::UpdateControls(PlayerControl* controls) {
	Vector3 forward = world->GetMainCamera()->GetForward();
	Vector3 right = world->GetMainCamera()->GetRight();
	Vector3 up = world->GetMainCamera()->GetUp();

	Vector3 linearMovement, animMovement;
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::W))
	{
		linearMovement += forward;
		animMovement.z = 1.0f;
	}
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::S))
	{
		linearMovement -= forward;
		animMovement.z = -1.0f;
	}

	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::A))
	{
		linearMovement -= right;
		animMovement.x = -1.0f;
	}
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::D))
	{
		linearMovement += right;
		animMovement.x = 1.0f;
	}

	controls->direction[0] = short(linearMovement.x * 10000);
	controls->direction[1] = short(linearMovement.y * 10000);
	controls->direction[2] = short(linearMovement.z * 10000);

	controls->camera[0] = (short)world->GetMainCamera()->GetPitch();
	controls->camera[1] = (short)world->GetMainCamera()->GetYaw();

	controls->aiming = Window::GetMouse()->ButtonHeld(MouseButtons::RIGHT);
	controls->shooting = controls->shooting || Window::GetMouse()->ButtonPressed(MouseButtons::LEFT);
	controls->jumping = controls->jumping || Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE);
}

PushdownState::PushdownResult ToonGame::OnUpdate(float dt, PushdownState** newState)
{
	if (m_ShouldQuitGame) { return PushdownResult::Pop; }
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE) || closeGame)
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

void ToonGame::ShowTime(float time) {
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
	Debug::Print(output, NCL::Maths::Vector2(0, 10));
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
