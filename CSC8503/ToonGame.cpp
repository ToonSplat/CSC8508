#include "GameWorld.h"
#include "PhysicsObject.h"

#include "ToonGame.h"

using namespace NCL;
using namespace CSC8503;

NCL::CSC8503::ToonGame::ToonGame()
{
	accumulator = 0.0f;

	physicsWorld = physicsCommon.createPhysicsWorld();
	physicsWorld->setGravity(reactphysics3d::Vector3(0.0f, -9.81f, 0.0f));
	world = new ToonGameWorld();
	renderer = new GameTechRenderer(*world);
	levelManager = new ToonLevelManager(*renderer, *physicsWorld, physicsCommon);

	//physics = new PhysicsSystem(*world);
	//physics->UseGravity(true);
}

NCL::CSC8503::ToonGame::~ToonGame()
{
	physicsCommon.destroyPhysicsWorld(physicsWorld);

	delete world;
	delete renderer;
	//delete physics;
	delete levelManager;
}

void NCL::CSC8503::ToonGame::UpdateGame(float dt)
{
	world->GetMainCamera()->UpdateCamera(dt);
	world->UpdateWorld(dt);

	renderer->Update(dt);

	accumulator += dt;
	while (accumulator >= timeStep)
	{
		physicsWorld->update(timeStep);
		accumulator -= timeStep;
	}
	//physics->Update(dt);
	levelManager->Update(dt);

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

	/*if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT))
	{
		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true))
			Debug::Print("Click Pos: " + std::to_string(closestCollision.collidedAt.x) + ", " + std::to_string(closestCollision.collidedAt.z), Vector2(5, 85));
	}*/
}
