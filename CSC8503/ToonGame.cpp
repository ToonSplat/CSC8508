#include "GameWorld.h"
#include "PhysicsObject.h"
#include "Camera.h"
#include "ToonGame.h"
#include "ToonUtils.h"
#include "ToonRaycastCallback.h"
#include "PaintBallClass.h"

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
	cameraTargetObject = levelManager->AddPlayerToWorld(Vector3(-20, 5, -20));

	//physics = new PhysicsSystem(*world);
	//physics->UseGravity(true);
}

NCL::CSC8503::ToonGame::~ToonGame()
{
	delete world;
	delete renderer;
	delete mainZone;
	for (auto& zone : *subZones)
		delete zone;
	delete subZones;
	physicsCommon.destroyPhysicsWorld(physicsWorld);
	//delete physics;
	delete levelManager;
}

void NCL::CSC8503::ToonGame::UpdateGame(float dt)
{
#pragma region To Be Changed
	Vector2 screenSize = Window::GetWindow()->GetScreenSize();
	Debug::Print("[]", Vector2(48.5, 50), Debug::RED);	//TODO: Hardcoded for now. To be changed later.
#pragma endregion

	if(cameraTargetObject)
		ToonGameWorld::Get()->GetMainCamera()->UpdateCamera(dt, ToonUtils::ConvertToNCLVector3(cameraTargetObject->GetRigidbody()->getTransform().getPosition()), 
			ToonUtils::ConvertToNCLVector3(cameraTargetObject->GetTransform().GetScale()));
	else
		ToonGameWorld::Get()->GetMainCamera()->UpdateCamera(dt);

	cameraTargetObject->Update(dt);
	cameraTargetObject->UpdateTargetObject(targetObject);
	ToonGameWorld::Get()->UpdateWorld(dt); // This doesn't actually do anything yet... will it?

	renderer->Update(dt);

	accumulator += dt;
	while (accumulator >= timeStep)
	{
		physicsWorld->update(timeStep);
		accumulator -= timeStep;
	}

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

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::P))
	{
		cameraTargetObject->GetRigidbody()->applyWorldForceAtCenterOfMass(reactphysics3d::Vector3(10.0f, 1000.0f, -10.0f));
		cameraTargetObject->GetRigidbody()->applyLocalTorque(reactphysics3d::Vector3(50.0f, 40.0f, -90.0f));
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
}
