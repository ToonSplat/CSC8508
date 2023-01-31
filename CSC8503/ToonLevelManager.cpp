#include "PhysicsObject.h"
#include "RenderObject.h"

#include "ToonLevelManager.h"
#include <stb/stb_image.h>

using namespace NCL;
using namespace CSC8503;

NCL::CSC8503::ToonLevelManager::ToonLevelManager(GameTechRenderer& renderer, PaintableZone* mainZone, std::vector<PaintableZone*>* subZones) :
	gameRenderer(renderer), mainZone(mainZone), subZones(subZones)
{
	if (!LoadAssets()) return;
	LoadLevel();
}

NCL::CSC8503::ToonLevelManager::~ToonLevelManager()
{
	delete cubeMesh;
	delete basicTex;
	delete basicShader;
}

bool NCL::CSC8503::ToonLevelManager::LoadAssets()
{
	//All Models
	if (!LoadModel(&cubeMesh, "cube.msh")) return false;
	if (!LoadModel(&charMesh, "goat.msh")) return false;

	//All Textures
	if (!LoadTexture(&basicTex, "Prefab_Grey50.png", true)) return false;
	if (!LoadTexture(&basicTexPurple, "Prefab_Purple.png", true)) return false;

	//All Shaders
	if (!LoadShader(&basicShader, "scene.vert", "scene.frag")) return false;

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
	axisObject = AddCubeToWorld(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), basicTex, nullptr, 0.0f);
	Debug::DrawAxisLines(axisObject->GetTransform().GetMatrix(), 2.0f, 100.0f);

	int XZ = Axes::X | Axes::Z;
	int XY = Axes::X | Axes::Y;
	int YZ = Axes::Y | Axes::Z;
	int XYZ = Axes::X | Axes::Y | Axes::Z;


	//Floors
	AddGridWorld(Axes(XZ), Vector3(10, 1, 10), 2, Vector3(-40, 0, -40), Vector3(2, 0.5f, 2), 0.0f, basicTex);
	AddGridWorld(Axes(XZ), Vector3(10, 1, 10), 2, Vector3(40, 0, -40), Vector3(2, 0.5f, 2), 0.0f, basicTex);
	AddGridWorld(Axes(XZ), Vector3(10, 1, 10), 2, Vector3(0, 0, 0), Vector3(2, 0.5f, 2), 0.0f, basicTex);
	AddGridWorld(Axes(XZ), Vector3(10, 1, 10), 2, Vector3(0, 0, -80), Vector3(2, 0.5f, 2), 0.0f, basicTex);

	////Walls Big
	//AddGridWorld(Axes(YZ), Vector3(1, 5, 10), 2, Vector3(78.0f, 2, -40), Vector3(0.5f, 2.0f, 2.0f), 0.0f, basicTex);
	//AddGridWorld(Axes(YZ), Vector3(1, 5, 10), 2, Vector3(-42.5f, 2, -40), Vector3(0.5f, 2.0f, 2.0f), 0.0f, basicTex);

	//AddGridWorld(Axes(XY), Vector3(10, 5, 1), 2, Vector3(0, 2, -82), Vector3(2.0f, 2.0f, 0.5f), 0.0f, basicTex);
	//AddGridWorld(Axes(XY), Vector3(10, 5, 1), 2, Vector3(0, 2, 38), Vector3(2.0f, 2.0f, 0.5f), 0.0f, basicTex);

	////Walls Small
	//AddGridWorld(Axes(XY), Vector3(10, 3, 1), 2, Vector3(-40.0f, 2, -42.0f), Vector3(2.0f, 2.0f, 0.5f), 0.0f, basicTex);
	//AddGridWorld(Axes(XY), Vector3(10, 3, 1), 2, Vector3(-40.0f, 2, -2.0f), Vector3(2.0f, 2.0f, 0.5f), 0.0f, basicTex);
	//AddGridWorld(Axes(XY), Vector3(10, 3, 1), 2, Vector3(40.0f, 2, -42.0f), Vector3(2.0f, 2.0f, 0.5f), 0.0f, basicTex);
	//AddGridWorld(Axes(XY), Vector3(10, 3, 1), 2, Vector3(40.0f, 2, -2.0f), Vector3(2.0f, 2.0f, 0.5f), 0.0f, basicTex);

	//AddGridWorld(Axes(YZ), Vector3(1, 3, 10), 2, Vector3(-2.0f, 2, -80.0f), Vector3(0.5f, 2.0f, 2.0f), 0.0f, basicTex);
	//AddGridWorld(Axes(YZ), Vector3(1, 3, 10), 2, Vector3(38.0f, 2, -80.0f), Vector3(0.5f, 2.0f, 2.0f), 0.0f, basicTex);
	//AddGridWorld(Axes(YZ), Vector3(1, 3, 10), 2, Vector3(-2.0f, 2, 0.0f), Vector3(0.5f, 2.0f, 2.0f), 0.0f, basicTex);
	//AddGridWorld(Axes(YZ), Vector3(1, 3, 10), 2, Vector3(38.0f, 2, 0.0f), Vector3(0.5f, 2.0f, 2.0f), 0.0f, basicTex);

	//Bridges
	AddGridWorld(Axes::X, Vector3(10, 1, 1), 2, Vector3(0.0f, 0.5f, -22.0f), Vector3(2.0f, 0.5f, 2.0f), 0.0f, basicTex);
	AddGridWorld(Axes::Z, Vector3(1, 1, 10), 2.0f, Vector3(18.0f, 0, -40.0f), Vector3(2.0f, 0.5f, 2.0f), 0.0f, basicTex);

	//Boxes
	AddGridWorld(Axes(XYZ), Vector3(2, 4, 2), 2.0f, Vector3(20.0f, 1.5f, 10.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, basicTexPurple);
	AddGridWorld(Axes(XYZ), Vector3(2, 2, 2), 2.0f, Vector3(15.0f, 1.5f, 10.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, basicTexPurple);
	AddGridWorld(Axes(XYZ), Vector3(2, 4, 2), 2.0f, Vector3(10.0f, 1.5f, 10.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, basicTexPurple);

	AddGridWorld(Axes(XYZ), Vector3(2, 2, 2), 2.0f, Vector3(-20.0f, 1.5f, -15.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, basicTexPurple);
	AddGridWorld(Axes(XYZ), Vector3(2, 2, 2), 2.0f, Vector3(-12.0f, 1.5f, -32.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, basicTexPurple);
	AddGridWorld(Axes(XYZ), Vector3(2, 2, 2), 2.0f, Vector3(-32.0f, 1.5f, -22.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, basicTexPurple);
	
	AddGridWorld(Axes(XYZ), Vector3(2, 2, 2), 2.0f, Vector3(70.0f, 1.5f, -12.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, basicTexPurple);
	AddGridWorld(Axes(XYZ), Vector3(2, 4, 2), 2.0f, Vector3(48.0f, 1.5f, -28.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, basicTexPurple);
	
	AddGridWorld(Axes(XYZ), Vector3(6, 4, 2), 2.0f, Vector3(12.0f, 1.5f, -60.0f), Vector3(1.0f, 1.0f, 1.0f), 0.0f, basicTexPurple);

	return true;
}

PaintableObject* NCL::CSC8503::ToonLevelManager::AddCubeToWorld(const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, TextureBase* cubeTex, PaintableZone* zone, float inverseMass)
{
	PaintableObject* cube;
	if(zone)
		cube = new PaintableObject(zone);
	else 
		cube = new PaintableObject();
	AABBVolume* volume = new AABBVolume(scale);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetOrientation(Quaternion::EulerAnglesToQuaternion(rotationEuler.x, rotationEuler.y, rotationEuler.z))
		.SetScale(scale * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, cubeTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	GameWorld::Get()->AddGameObject(cube);

	return cube;
}

void NCL::CSC8503::ToonLevelManager::AddGridWorld(Axes axes, const Vector3& gridSize, const float& gridSpacing, const Vector3& gridPosition, const Vector3& cubeScale, const float& cubeMass, TextureBase* cubeTex)
{
	PaintableZone* tempZone = new PaintableZone(mainZone);
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

				AddCubeToWorld(newPos, Vector3(0, 0, 0), cubeScale, cubeTex, tempZone, 0.0f);
			}
		}
	}
	subZones->push_back(tempZone);
}

Player* ToonLevelManager::AddMoveablePlayer(const Vector3& position) {
	float meshSize = 1.0f;
	float inverseMass = 0.5f;

	Player* character = new Player();
	SphereVolume* volume = new SphereVolume(1.0f);

	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), charMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	GameWorld::Get()->AddGameObject(character);
	//player = character;

	return character;
}