#include "GameWorld.h"
#include "Camera.h"
#include "ToonGame.h"
#include "ToonUtils.h"
#include "ToonFollowCamera.h"
#include "ToonRaycastCallback.h"
#include "PaintBallClass.h"

using namespace NCL;
using namespace CSC8503;

NCL::CSC8503::ToonGame::ToonGame()
{
	world = new ToonGameWorld();	
	renderer = new GameTechRenderer(*world);
	
	levelManager = new ToonLevelManager(*renderer);
	player = levelManager->AddPlayerToWorld(Vector3(-20, 5, -20));
	
	followCamera = new ToonFollowCamera(*player);
	world->SetMainCamera(followCamera);

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
	Debug::Print("[]", Vector2(48.5, 50), Debug::RED);	//TODO: Hardcoded for now. To be changed later.
#pragma endregion

	world->GetMainCamera()->UpdateCamera(dt);
	world->UpdateWorld(dt);
	player->Update(dt);

	renderer->Update(dt);

	accumulator += dt;
	while (accumulator >= timeStep)
	{
		world->GetPhysicsWorld().update(timeStep);
		accumulator -= timeStep;
	}
  
	levelManager->Update(dt);

	renderer->Render();
	//Debug::UpdateRenderables(dt);

	//UpdateTesting();
}

void NCL::CSC8503::ToonGame::UpdateCamera(float dt)
{
	ToonGameWorld::Get()->GetMainCamera()->UpdateCamera(dt, ToonUtils::ConvertToNCLVector3(player->GetRigidbody()->getTransform().getPosition()),
		ToonUtils::ConvertToNCLVector3(player->GetTransform().GetScale()));
	float horizontalAngle = ToonGameWorld::Get()->GetMainCamera()->GetYaw();
	float verticalAngle = ToonGameWorld::Get()->GetMainCamera()->GetPitch() + 20;

	Matrix4 view = ToonGameWorld::Get()->GetMainCamera()->BuildViewMatrix();
	Matrix4 cam = view.Inverse();
	//ToonGameWorld::Get()->GetMainCamera()->UpdateCamera(dt);

	player->Update(cam, horizontalAngle, verticalAngle, dt);
	player->UpdateTargetObject(targetObject);
	ToonGameWorld::Get()->UpdateWorld(dt);
}

void NCL::CSC8503::ToonGame::UpdateTesting()
{
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
		reactphysics3d::Vector3 endRay = ToonUtils::ConvertToRP3DVector3(end);
		reactphysics3d::Ray ray(ToonUtils::ConvertToRP3DVector3(world->GetMainCamera()->GetPosition()), endRay);
		ToonRaycastCallback rayCallback;

		ToonGameWorld::Get()->GetPhysicsWorld().raycast(ray, &rayCallback);

		Debug::DrawLine(world->GetMainCamera()->GetPosition(), rayCallback.GetHitWorldPos(), Debug::YELLOW, 10.0f);
		Debug::DrawLine(rayCallback.GetHitWorldPos(), rayCallback.GetHitWorldPos() + rayCallback.GetHitNormal(), Debug::RED, 10.0f);
	}

	/*if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT))
	{
		Ray ray = CollisionDetection::BuildRayFromCenter(*world->GetMainCamera());//BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		/*if (ToonGameWorld::Get()->Raycast(ray, closestCollision, true))
		{
			targetObject = (GameObject*)closestCollision.node;
			Debug::Print("Click Pos: " + std::to_string(closestCollision.collidedAt.x) + ", " + std::to_string(closestCollision.collidedAt.z), Vector2(5, 85));
		}
	}*/
}
