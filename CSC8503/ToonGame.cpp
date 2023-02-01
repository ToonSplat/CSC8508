#include "GameWorld.h"
#include "PhysicsObject.h"

#include "ToonGame.h"
#include "ToonUtils.h"
#include "ToonRaycastCallback.h"

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

	if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT))
	{
		Vector3 end = world->GetMainCamera()->GetPosition() + world->GetMainCamera()->GetForward() * 500.0f;
		std::cout << "End: " << end << std::endl;
		reactphysics3d::Vector3 endRay = ToonUtils::ConvertToRP3DVector3(end);
		reactphysics3d::Ray ray(ToonUtils::ConvertToRP3DVector3(world->GetMainCamera()->GetPosition()), endRay);
		ToonRaycastCallback rayCallback;

		physicsWorld->raycast(ray, &rayCallback);
		Debug::Print("Click Pos: " + std::to_string(rayCallback.GetHitWorldPos().x) + ", " + std::to_string(rayCallback.GetHitWorldPos().z), Vector2(5, 85));

		Debug::DrawLine(world->GetMainCamera()->GetPosition(), rayCallback.GetHitWorldPos(), Debug::YELLOW, 10.0f);
		Debug::DrawLine(rayCallback.GetHitWorldPos(), rayCallback.GetHitWorldPos() + rayCallback.GetHitNormal(), Debug::RED, 10.0f);
	}

	/*if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT))
	{
		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true))
			Debug::Print("Click Pos: " + std::to_string(closestCollision.collidedAt.x) + ", " + std::to_string(closestCollision.collidedAt.z), Vector2(5, 85));
	}*/
}
