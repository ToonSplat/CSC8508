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

	ToonGameWorld::Get()->GetMainCamera()->UpdateCamera(dt, ReactVec3ToNcl(cameraTargetObject->GetRigidbody()->getTransform().getPosition()), 
		ReactVec3ToNcl(cameraTargetObject->GetTransform().GetScale()));
	float horizontalAngle = ToonGameWorld::Get()->GetMainCamera()->GetYaw();
	float verticalAngle = ToonGameWorld::Get()->GetMainCamera()->GetPitch() + 20;

	Matrix4 view = ToonGameWorld::Get()->GetMainCamera()->BuildViewMatrix();
	Matrix4 cam = view.Inverse();
	//ToonGameWorld::Get()->GetMainCamera()->UpdateCamera(dt);

	cameraTargetObject->Update(cam, horizontalAngle, verticalAngle, dt);
	cameraTargetObject->UpdateTargetObject(targetObject);
	ToonGameWorld::Get()->UpdateWorld(dt);

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
		Ray ray = CollisionDetection::BuildRayFromCenter(*world->GetMainCamera());//BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		/*if (ToonGameWorld::Get()->Raycast(ray, closestCollision, true))
		{
			targetObject = (GameObject*)closestCollision.node;
			Debug::Print("Click Pos: " + std::to_string(closestCollision.collidedAt.x) + ", " + std::to_string(closestCollision.collidedAt.z), Vector2(5, 85));
		}*/
	}
}

Vector3 ToonGame::ReactVec3ToNcl(reactphysics3d::Vector3 vector3)
{
	Vector3 ret = Vector3(vector3.x, vector3.y, vector3.z);
	return ret;
}
