#include "PhysicsObject.h"
#include "RenderObject.h"

#include "ToonLevelManager.h"
#include <stb/stb_image.h>

using namespace NCL;
using namespace CSC8503;

NCL::CSC8503::ToonLevelManager::ToonLevelManager(GameTechRenderer& renderer) :
	gameRenderer(renderer)
{
	if (!LoadAssets()) return;
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
	if (!LoadModel(cubeMesh, "cube.msh")) return false;

	//All Textures
	if (!LoadTexture(basicTex, "checkerboard.png")) return false;

	//All Shaders
	if (!LoadShader(basicShader, "scene.vert", "scene.frag")) return false;

	return true;
}

bool NCL::CSC8503::ToonLevelManager::LoadModel(MeshGeometry* mesh, const std::string& meshFileName)
{
	mesh = gameRenderer.LoadMesh(meshFileName);
	if (mesh == nullptr)
		return false;

	return true;
}

bool NCL::CSC8503::ToonLevelManager::LoadTexture(TextureBase* tex, const std::string& textureFileName, const bool& invert)
{
	if (invert)
	{
		stbi_set_flip_vertically_on_load(true);
		tex = gameRenderer.LoadTexture(textureFileName);
		stbi_set_flip_vertically_on_load(false);
	}
	else
		tex = gameRenderer.LoadTexture(textureFileName);

	if (tex == nullptr)
		return false;

	return true;
}

bool NCL::CSC8503::ToonLevelManager::LoadShader(ShaderBase* shader, const std::string& shaderVertexShader, const std::string& shaderFragmentShader)
{
	shader = gameRenderer.LoadShader(shaderVertexShader, shaderFragmentShader);
	if (shader == nullptr)
		return false;

	return true;
}

bool NCL::CSC8503::ToonLevelManager::LoadLevel()
{
	return false;
}

GameObject* NCL::CSC8503::ToonLevelManager::AddCubeToWorld(const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, TextureBase* cubeTex, float inverseMass)
{
	GameObject* cube = new GameObject();

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

void NCL::CSC8503::ToonLevelManager::InitCubeGridWorld(const int& numRows, const int& numCols, const float& rowSpacing, const float& colSpacing, const Vector3& cubeScale, const float& cubeMass)
{
}
