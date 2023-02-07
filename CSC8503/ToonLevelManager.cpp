#include "PhysicsObject.h"
#include "RenderObject.h"
#include "ToonRenderObject.h"
#include "ToonGameObject.h"
#include "ToonLevelManager.h"
#include <stb/stb_image.h>
#include <reactphysics3d/reactphysics3d.h>

using namespace NCL;
using namespace CSC8503;

ToonLevelManager* ToonLevelManager::instance = nullptr;

NCL::CSC8503::ToonLevelManager::ToonLevelManager(GameTechRenderer& renderer) :
	gameRenderer(renderer)
{
	instance = this;
	if (!LoadAssets()) return;

	axisObject = AddCubeToWorld(Vector3(0, 10, 0), Vector3(0, 0, 0), Vector3(4, 1, 1), GetTexture("basicPurple"), 1.0f);
	//axisObject = AddSphereToWorld(Vector3(0, 10, 0), Vector3(0, 0, 0), 2.0f, basicTexPurple, 1.0f);
	axisObject->GetRigidbody()->setType(reactphysics3d::BodyType::DYNAMIC);
	//Debug::DrawAxisLines(axisObject->GetTransform().GetMatrix(), 2.0f, 100.0f);

	LoadLevel();
}

NCL::CSC8503::ToonLevelManager::~ToonLevelManager()
{
	for (auto& mesh : meshMap)
		delete mesh.second;
	for (auto& texture : textureMap)
		delete texture.second;
	for (auto& shader : shaderMap)
		delete shader.second;
}

void NCL::CSC8503::ToonLevelManager::Update(float dt)
{
	//if (axisObject)
		//std::cout << "Position: " << axisObject->GetRigidbody()->getTransform().getPosition().x << ", " << axisObject->GetRigidbody()->getTransform().getPosition().y << ", " << axisObject->GetRigidbody()->getTransform().getPosition().z << std::endl;

	if (axisObject)
	{
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::N))
		{
			axisObject->GetRigidbody()->applyWorldForceAtCenterOfMass(reactphysics3d::Vector3(0, 1000.0f, 0));
			axisObject->GetRigidbody()->applyLocalTorque(reactphysics3d::Vector3(50.0f, 40.0f, -90.0f));
		}

		static bool scaled = false;
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::M))
		{
			axisObject->SetPosition(reactphysics3d::Vector3(0, 40, 0));
			axisObject->SetOrientation(Vector3(0.0f, 0.0f, 0.0f));
			scaled = !scaled;
			axisObject->SetScale(scaled ? Vector3(1, 1, 1) : Vector3(4, 1, 1));
			//axisObject->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(90.0f, 0.0f, 0.0f));
		}

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::B))
		{
			Vector3 end = ToonGameWorld::Get()->GetMainCamera()->GetPosition() + ToonGameWorld::Get()->GetMainCamera()->GetForward() * 4.0f;
			axisObject->SetPosition(end);
			axisObject->GetRigidbody()->setLinearVelocity(reactphysics3d::Vector3(0, 0, 0));
		}		
		//ToonGameWorld::Get()->GetMainCamera()->GetPosition();
	}
}

bool NCL::CSC8503::ToonLevelManager::LoadAssets()
{
	//All Models
	if (!LoadModel(&meshMap["cube"], "cube.msh")) return false;
	if (!LoadModel(&meshMap["goat"], "goat.msh")) return false;
	if (!LoadModel(&meshMap["sphere"], "sphere.msh")) return false;

	//All Textures
	if (!LoadTexture(&textureMap["mesh"], "checkerboard.png", false)) return false;
	if (!LoadTexture(&textureMap["basic"], "Prefab_Grey50.png", true)) return false;
	if (!LoadTexture(&textureMap["basicPurple"], "Prefab_Purple.png", true)) return false;

	//All Shaders
	if (!LoadShader(&shaderMap["basic"], "scene.vert", "scene.frag")) return false;

	return true;
}

bool NCL::CSC8503::ToonLevelManager::LoadModel(MeshGeometry** mesh, const std::string& meshFileName)
{
	*mesh = gameRenderer.LoadMesh(meshFileName);
	if (mesh == nullptr)
		return false;

	return true;
}

bool NCL::CSC8503::ToonLevelManager::LoadTexture(TextureBase** tex, const std::string& textureFileName, const bool& invert)
{
	if (invert)
	{
		stbi_set_flip_vertically_on_load(true);
		*tex = gameRenderer.LoadTexture(textureFileName);
		stbi_set_flip_vertically_on_load(false);
	}
	else
		*tex = gameRenderer.LoadTexture(textureFileName);

	if (*tex == nullptr)
		return false;

	return true;
}

bool NCL::CSC8503::ToonLevelManager::LoadShader(ShaderBase** shader, const std::string& shaderVertexShader, const std::string& shaderFragmentShader)
{
	*shader = gameRenderer.LoadShader(shaderVertexShader, shaderFragmentShader);
	if (*shader == nullptr)
		return false;

	return true;
}

bool NCL::CSC8503::ToonLevelManager::LoadLevel()
{
	int XZ = Axes::X | Axes::Z;
	int XY = Axes::X | Axes::Y;
	int YZ = Axes::Y | Axes::Z;
	int XYZ = Axes::X | Axes::Y | Axes::Z;

	//Floors
	AddGridWorld(Axes(XZ), Vector3(10, 1, 10), 2, Vector3(-40, 0, -40), Vector3(2, 0.5f, 2), 0.0f, GetTexture("basic"));
	AddGridWorld(Axes(XZ), Vector3(10, 1, 10), 2, Vector3(40, 0, -40), Vector3(2, 0.5f, 2), 0.0f, GetTexture("basic"));
	AddGridWorld(Axes(XZ), Vector3(10, 1, 10), 2, Vector3(0, 0, 0), Vector3(2, 0.5f, 2), 0.0f, GetTexture("basic"));
	AddGridWorld(Axes(XZ), Vector3(10, 1, 10), 2, Vector3(0, 0, -80), Vector3(2, 0.5f, 2), 0.0f, GetTexture("basic"));

	//Walls Big
	AddGridWorld(Axes(YZ), Vector3(1, 5, 10), 2, Vector3(78.0f, 2, -40), Vector3(0.5f, 2.0f, 2.0f), 0.0f, GetTexture("basic"));
	AddGridWorld(Axes(YZ), Vector3(1, 5, 10), 2, Vector3(-42.5f, 2, -40), Vector3(0.5f, 2.0f, 2.0f), 0.0f, GetTexture("basic"));

	AddGridWorld(Axes(XY), Vector3(10, 5, 1), 2, Vector3(0, 2, -82), Vector3(2.0f, 2.0f, 0.5f), 0.0f, GetTexture("basic"));
	AddGridWorld(Axes(XY), Vector3(10, 5, 1), 2, Vector3(0, 2, 38), Vector3(2.0f, 2.0f, 0.5f), 0.0f, GetTexture("basic"));

	//Walls Small
	AddGridWorld(Axes(XY), Vector3(10, 3, 1), 2, Vector3(-40.0f, 2, -42.0f), Vector3(2.0f, 2.0f, 0.5f), 0.0f, GetTexture("basic"));
	AddGridWorld(Axes(XY), Vector3(10, 3, 1), 2, Vector3(-40.0f, 2, -2.0f), Vector3(2.0f, 2.0f, 0.5f), 0.0f, GetTexture("basic"));
	AddGridWorld(Axes(XY), Vector3(10, 3, 1), 2, Vector3(40.0f, 2, -42.0f), Vector3(2.0f, 2.0f, 0.5f), 0.0f, GetTexture("basic"));
	AddGridWorld(Axes(XY), Vector3(10, 3, 1), 2, Vector3(40.0f, 2, -2.0f), Vector3(2.0f, 2.0f, 0.5f), 0.0f, GetTexture("basic"));

	AddGridWorld(Axes(YZ), Vector3(1, 3, 10), 2, Vector3(-2.0f, 2, -80.0f), Vector3(0.5f, 2.0f, 2.0f), 0.0f, GetTexture("basic"));
	AddGridWorld(Axes(YZ), Vector3(1, 3, 10), 2, Vector3(38.0f, 2, -80.0f), Vector3(0.5f, 2.0f, 2.0f), 0.0f, GetTexture("basic"));
	AddGridWorld(Axes(YZ), Vector3(1, 3, 10), 2, Vector3(-2.0f, 2, 0.0f), Vector3(0.5f, 2.0f, 2.0f), 0.0f, GetTexture("basic"));
	AddGridWorld(Axes(YZ), Vector3(1, 3, 10), 2, Vector3(38.0f, 2, 0.0f), Vector3(0.5f, 2.0f, 2.0f), 0.0f, GetTexture("basic"));

	//Bridges
	AddGridWorld(Axes::X, Vector3(10, 1, 1), 2, Vector3(0.0f, 0.5f, -22.0f), Vector3(2.0f, 0.5f, 2.0f), 0.0f, GetTexture("basic"));
	AddGridWorld(Axes::Z, Vector3(1, 1, 10), 2, Vector3(18.0f, 0, -40.0f), Vector3(2.0f, 0.5f, 2.0f), 0.0f, GetTexture("basic"));

	//Boxes
	AddGridWorld(Axes(XYZ), Vector3(2, 4, 2), 2.0f, Vector3(20.0f, 1.5f, 10.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, GetTexture("basicPurple"));
	AddGridWorld(Axes(XYZ), Vector3(2, 2, 2), 2.0f, Vector3(15.0f, 1.5f, 10.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, GetTexture("basicPurple"));
	AddGridWorld(Axes(XYZ), Vector3(2, 4, 2), 2.0f, Vector3(10.0f, 1.5f, 10.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, GetTexture("basicPurple"));

	AddGridWorld(Axes(XYZ), Vector3(2, 2, 2), 2.0f, Vector3(-20.0f, 1.5f, -15.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, GetTexture("basicPurple"));
	AddGridWorld(Axes(XYZ), Vector3(2, 2, 2), 2.0f, Vector3(-12.0f, 1.5f, -32.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, GetTexture("basicPurple"));
	AddGridWorld(Axes(XYZ), Vector3(2, 2, 2), 2.0f, Vector3(-32.0f, 1.5f, -22.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, GetTexture("basicPurple"));
	
	AddGridWorld(Axes(XYZ), Vector3(2, 2, 2), 2.0f, Vector3(70.0f, 1.5f, -12.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, GetTexture("basicPurple"));
	AddGridWorld(Axes(XYZ), Vector3(2, 4, 2), 2.0f, Vector3(48.0f, 1.5f, -28.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, GetTexture("basicPurple"));
	
	AddGridWorld(Axes(XYZ), Vector3(6, 4, 2), 2.0f, Vector3(12.0f, 1.5f, -60.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, GetTexture("basicPurple"));

	return true;
}


ToonGameObject* NCL::CSC8503::ToonLevelManager::AddCubeToWorld(const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, TextureBase* cubeTex, float mass)
{
	ToonGameObject* cube = new ToonGameObject(ToonGameWorld::Get()->GetPhysicsWorld());

	cube->GetTransform().SetPosition(position).
		SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z)).
		SetScale(scale * 2.0f);

	//std::cout << cube->GetTransform().GetMatrix() << std::endl;

	cube->AddRigidbody();
	cube->GetRigidbody()->setType(reactphysics3d::BodyType::STATIC);
	cube->GetRigidbody()->setMass(mass);
	cube->GetRigidbody()->setIsAllowedToSleep(true);
	cube->SetRenderObject(new ToonRenderObject(&cube->GetTransform(), GetMesh("cube"), cubeTex, GetShader("basic")));

	const reactphysics3d::Vector3 boxExtent(scale.x, scale.y, scale.z);
	reactphysics3d::BoxShape* cubeBoxShape = ToonGameWorld::Get()->GetPhysicsCommon().createBoxShape(boxExtent);
	cube->SetCollisionShape(cubeBoxShape);

	//reactphysics3d::Collider* cubeCollider = cube->GetRigidbody()->addCollider(cubeBoxShape, reactphysics3d::Transform::identity());

	cube->SetCollider(cubeBoxShape);
	cube->GetCollider()->getMaterial().setBounciness(0.1f);

	ToonGameWorld::Get()->AddGameObject(cube);

	return cube;
}

ToonGameObject* NCL::CSC8503::ToonLevelManager::AddSphereToWorld(const Vector3& position, const Vector3& rotationEuler, const float& radius, TextureBase* sphereTex, float mass)
{
	ToonGameObject* sphere = new ToonGameObject(ToonGameWorld::Get()->GetPhysicsWorld());

	sphere->GetTransform().SetPosition(position).
		SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z)).
		SetScale(Vector3(radius, radius, radius));

	//std::cout << cube->GetTransform().GetMatrix() << std::endl;

	sphere->AddRigidbody();
	sphere->GetRigidbody()->setType(reactphysics3d::BodyType::STATIC);
	sphere->GetRigidbody()->setIsAllowedToSleep(true);

	sphere->SetRenderObject(new ToonRenderObject(&sphere->GetTransform(), GetMesh("sphere"), sphereTex, GetShader("basic")));

	reactphysics3d::SphereShape* sphereShape = ToonGameWorld::Get()->GetPhysicsCommon().createSphereShape(radius * 0.85f);
	sphere->SetCollisionShape(sphereShape);

	//reactphysics3d::Collider* cubeCollider = cube->GetRigidbody()->addCollider(cubeBoxShape, reactphysics3d::Transform::identity());

	sphere->SetCollider(sphereShape);
	sphere->GetCollider()->getMaterial().setBounciness(0.1f);

	ToonGameWorld::Get()->AddGameObject(sphere);

	return sphere;
}

void NCL::CSC8503::ToonLevelManager::AddGridWorld(Axes axes, const Vector3& gridSize, const float& gridSpacing, const Vector3& gridPosition, const Vector3& cubeScale, const float& cubeMass, TextureBase* cubeTex)
{
	SetSelectedAxes(axes);
	for (int x = 0; x < gridSize.x && (gridSize.x > 0); x++)
	{
		for (int y = 0; y < gridSize.y && (gridSize.y > 0); y++)
		{
			for (int z = 0; z < gridSize.z && (gridSize.z > 0); z++)
			{
				Vector3 newPos;
				newPos.x = (IsXSelected() ? x * gridSpacing * cubeScale.x : 0.0f) + gridPosition.x;
				newPos.y = (IsYSelected() ? y * gridSpacing * cubeScale.y : 0.0f) + gridPosition.y;
				newPos.z = (IsZSelected() ? z * gridSpacing * cubeScale.z : 0.0f) + gridPosition.z;

				AddCubeToWorld(newPos, Vector3(0, 0, 0), cubeScale, cubeTex, 0.0f);
			}
		}
	}
}

Player* ToonLevelManager::AddPlayerToWorld(const Vector3& position) 
{
	Player* player = new Player(ToonGameWorld::Get()->GetPhysicsWorld(), position, Vector3(0, 0, 0), 2.0f);
	player->SetRenderObject(new ToonRenderObject(&player->GetTransform(), GetMesh("goat"), GetTexture("basicPurple"), GetShader("basic")));

	return player;
}