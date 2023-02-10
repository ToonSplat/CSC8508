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
}

void NCL::CSC8503::ToonGame::UpdateGame(float dt)
{

#pragma region To Be Changed
	Vector2 screenSize = Window::GetWindow()->GetScreenSize();
	Debug::Print("[]", Vector2(48.5f, 50.0f), Debug::RED);	//TODO: Hardcoded for now. To be changed later.
#pragma endregion

	world->GetMainCamera()->UpdateCamera(dt);
	if(world->GetMinimapCamera()) world->GetMinimapCamera()->UpdateCamera(dt);
	world->UpdateWorld(dt);
	
	if(player) player->Update(dt);

	accumulator += dt;
	while (accumulator >= timeStep)
	{
		world->GetPhysicsWorld().update(timeStep);
		accumulator -= timeStep;
		world->DeleteObjects();
	}

	world->interpolationFactor = float(accumulator / timeStep);

	renderer->Render();
	Debug::UpdateRenderables(dt);
}
