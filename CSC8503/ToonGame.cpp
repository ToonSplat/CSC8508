#include "GameWorld.h"
#include "PhysicsObject.h"

#include "ToonGame.h"
#include "PaintBallClass.h"

using namespace NCL;
using namespace CSC8503;

NCL::CSC8503::ToonGame::ToonGame()
{
	world = new GameWorld();
	renderer = new GameTechRenderer(*world);

	physics = new PhysicsSystem(*world);
	physics->UseGravity(false);

	mainZone = new PaintableZone();
	subZones = new std::vector<PaintableZone*>;

	levelManager = new ToonLevelManager(*renderer, mainZone, subZones);

	cameraTargetObject = levelManager->AddMoveablePlayer(Vector3(-20, 5, -20), world);
}

NCL::CSC8503::ToonGame::~ToonGame()
{
	delete world;
	delete renderer;
	delete physics;
	delete mainZone;
	for (auto& zone : *subZones)
		delete zone;
	delete subZones;
	delete levelManager;
}

void NCL::CSC8503::ToonGame::UpdateGame(float dt)
{
#pragma region To Be Changed
	Vector2 screenSize = Window::GetWindow()->GetScreenSize();
	Debug::Print("[]", Vector2(48.5, 51), Debug::RED);	//TODO: Hardcoded for now. To be changed later.
#pragma endregion
	world->GetMainCamera()->UpdateCamera(dt);
	world->UpdateWorld(dt);

	world->GetMainCamera()->UpdateCamera(dt, cameraTargetObject->GetTransform().GetPosition(), cameraTargetObject->GetTransform().GetScale());
	float horizontalAngle = world->GetMainCamera()->GetYaw();
	float verticalAngle = world->GetMainCamera()->GetPitch() + 20;

	Matrix4 view = world->GetMainCamera()->BuildViewMatrix();
	Matrix4 cam = view.Inverse();

	cameraTargetObject->Update(cam, horizontalAngle, verticalAngle, dt);
	cameraTargetObject->UpdateTargetObject(targetObject);

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
		Ray ray = CollisionDetection::BuildRayFromCenter(*world->GetMainCamera());//BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true))
		{
			targetObject = (GameObject*)closestCollision.node;
			Debug::Print("Click Pos: " + std::to_string(closestCollision.collidedAt.x) + ", " + std::to_string(closestCollision.collidedAt.z), Vector2(5, 85));
		}
	}
	/*mainZone->PrintOwnership();
	for (auto& zone : *subZones)
		zone->PrintOwnership();*/
}

//void NCL::CSC8503::ToonGame::UpdateObjects(ShaderBase* basicShad, MeshGeometry* sphMesh)
//{
//	basicShader = basicShad;
//	sphereMesh  = sphMesh;
//	cameraTargetObject->UpdateObjects(basicShader, sphereMesh, world);
//
//}
