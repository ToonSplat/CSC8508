#include "GameWorld.h"
#include "Camera.h"
#include "ToonGame.h"
#include "ToonUtils.h"
#include "ToonFollowCamera.h"
#include "ToonMinimapCamera.h"
#include "ToonRaycastCallback.h"
#include "PaintBallClass.h"

using namespace NCL;
using namespace CSC8503;


ToonGame::ToonGame(bool offline) : offline(offline)
{
	world = new ToonGameWorld();	
	renderer = new GameTechRenderer(*world);
	
	levelManager = new ToonLevelManager(*renderer);
	baseWeapon = new PaintBallClass(15, 500, 0.5f, 1.0f, 5, levelManager->GetShader("basic"), levelManager->GetMesh("sphere"));
	if (offline) {
		player = levelManager->AddPlayerToWorld(Vector3(20, 5, 0), world->GetTeamLeastPlayers());
		playerControl = new PlayerControl();
		player->SetWeapon(baseWeapon);
		world->SetMainCamera(new ToonFollowCamera(player));
		world->SetMinimapCamera(new ToonMinimapCamera(*player));
	}
	else {
		world->SetMainCamera(new Camera());
	}

	accumulator = 0.0f;
	showCursor = false;
}

NCL::CSC8503::ToonGame::~ToonGame()
{
	delete world;
	delete renderer;
	delete baseWeapon;
	delete levelManager;
	delete playerControl;
}

void NCL::CSC8503::ToonGame::UpdateGame(float dt)
{

#pragma region To Be Changed
	Vector2 screenSize = Window::GetWindow()->GetScreenSize();
	Debug::Print("[]", Vector2(48.5f, 50.0f), Debug::RED);	//TODO: Hardcoded for now. To be changed later.
#pragma endregion

	world->GetMainCamera()->UpdateCamera(dt);
	if(world->GetMinimapCamera())
		world->GetMinimapCamera()->UpdateCamera(dt);
	world->UpdateWorld(dt);

	if (offline) {
		UpdateControls(playerControl);
	}
	if (player) {
		player->Update(dt); 
		if (offline) {
			player->MovementUpdate(dt, playerControl);
		}
		// This next line is an abomination and should be refactored by Ryan
		else {
			player->SetAiming(playerControl->aiming);
		}
	}

	accumulator += dt;
	while (accumulator >= timeStep)
	{
		world->GetPhysicsWorld().update(reactphysics3d::decimal(timeStep));
		accumulator -= timeStep;
		world->DeleteObjects();
	}

	renderer->Render();
	Debug::UpdateRenderables(dt);
}

void ToonGame::UpdateControls(PlayerControl* controls) {
	Vector3 forward = ToonGameWorld::Get()->GetMainCamera()->GetForward();
	Vector3 right = ToonGameWorld::Get()->GetMainCamera()->GetRight();
	Vector3 up = ToonGameWorld::Get()->GetMainCamera()->GetUp();

	Vector3 linearMovement;
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::W)) linearMovement += forward;
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::S)) linearMovement -= forward;

	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::A)) linearMovement -= right;
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::D)) linearMovement += right;

	controls->direction[0] = short(linearMovement.x * 1000);
	controls->direction[1] = short(linearMovement.y * 1000);
	controls->direction[2] = short(linearMovement.z * 1000);

	controls->camera[0] = ToonGameWorld::Get()->GetMainCamera()->GetPitch();
	controls->camera[1] = ToonGameWorld::Get()->GetMainCamera()->GetYaw();

	controls->aiming = Window::GetMouse()->ButtonHeld(MouseButtons::RIGHT);
	controls->shooting = Window::GetMouse()->ButtonHeld(MouseButtons::LEFT);
	controls->jumping = Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE);
}
