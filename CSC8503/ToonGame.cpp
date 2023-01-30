#include "GameWorld.h"
#include "PhysicsObject.h"

#include "ToonGame.h"

using namespace NCL;
using namespace CSC8503;

NCL::CSC8503::ToonGame::ToonGame()
{
	world = new GameWorld();
	renderer = new GameTechRenderer(*world);

	physics = new PhysicsSystem(*world);
	physics->UseGravity(true);

	levelManager = new ToonLevelManager(*renderer);
}

NCL::CSC8503::ToonGame::~ToonGame()
{
	delete world;
	delete renderer;
	delete physics;
	delete levelManager;
}

void NCL::CSC8503::ToonGame::UpdateGame(float dt)
{
	world->GetMainCamera()->UpdateCamera(dt);
	world->UpdateWorld(dt);

	renderer->Update(dt);
	physics->Update(dt);

	renderer->Render();
	Debug::UpdateRenderables(dt);

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) 
	{
		showCursor = !showCursor;
		if (showCursor) 
		{
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else 
		{
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}

	if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) 
	{
		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true))
			Debug::Print("Click Pos: " + std::to_string(closestCollision.collidedAt.x) + ", " + std::to_string(closestCollision.collidedAt.z), Vector2(5, 85));
	}
}
