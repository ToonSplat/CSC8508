#include "PhysicsObject.h"
#include "RenderObject.h"
#include "ToonRenderObject.h"
#include "ToonGameObject.h"
#include "ToonLevelManager.h"
#include <stb/stb_image.h>
#include <reactphysics3d/reactphysics3d.h>
#include "ToonGameWorld.h"

using namespace NCL;
using namespace CSC8503;

NCL::CSC8503::ToonLevelManager::ToonLevelManager(GameTechRenderer* renderer, ToonGameWorld* gameWorld) :
	gameRenderer(renderer), gameWorld(gameWorld)
{
	if (!LoadAssets()) return;
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
	
}

bool NCL::CSC8503::ToonLevelManager::LoadAssets()
{
	//All Models
	if (!LoadModel(&meshMap["cube"], "cube.msh")) return false;
	if (!LoadModel(&meshMap["arrow"], "Minimap_Arrow.msh")) return false;
	if (!LoadModel(&meshMap["player"], "Character_Boss.msh")) return false;
	if (!LoadModel(&meshMap["sphere"], "sphere.msh")) return false;
	if (!LoadModel(&meshMap["floorMain"], "FloorsMain.msh")) return false;
	if (!LoadModel(&meshMap["platformMain"], "Level_Platform.msh")) return false;

	//All Textures
	if (!LoadTexture(&textureMap["mesh"], "checkerboard.png", false)) return false;
	if (!LoadTexture(&textureMap["basic"], "Prefab_Grey50.png", true)) return false;
	if (!LoadTexture(&textureMap["basicPurple"], "Prefab_Purple.png", true)) return false;
	if (!LoadTexture(&textureMap["boss_player"], "Boss_diffuse.png", true)) return false;

	//All Shaders
	if (!LoadShader(&shaderMap["basic"], "scene.vert", "scene.frag")) return false;
	if (!LoadShader(&shaderMap["animated"], "sceneSkin.vert", "scene.frag")) return false;

	return true;
}

bool NCL::CSC8503::ToonLevelManager::LoadModel(MeshGeometry** mesh, const std::string& meshFileName)
{
	*mesh = gameRenderer->LoadMesh(meshFileName);
	if (mesh == nullptr)
		return false;

	return true;
}

bool NCL::CSC8503::ToonLevelManager::LoadTexture(TextureBase** tex, const std::string& textureFileName, const bool& invert)
{
	if (invert)
	{
		stbi_set_flip_vertically_on_load(true);
		*tex = gameRenderer->LoadTexture(textureFileName);
		stbi_set_flip_vertically_on_load(false);
	}
	else
		*tex = gameRenderer->LoadTexture(textureFileName);

	if (*tex == nullptr)
		return false;

	return true;
}

bool NCL::CSC8503::ToonLevelManager::LoadShader(ShaderBase** shader, const std::string& shaderVertexShader, const std::string& shaderFragmentShader)
{
	*shader = gameRenderer->LoadShader(shaderVertexShader, shaderFragmentShader);
	if (*shader == nullptr)
		return false;

	return true;
}

void ToonLevelManager::ResetLevel(std::vector<ToonNetworkObject*>* networkObjectList) {
	gameWorld->ClearAndErase();
	LoadPrototypeLevel(networkObjectList);
}

bool NCL::CSC8503::ToonLevelManager::LoadPrototypeLevel(std::vector<ToonNetworkObject*>* networkObjectList)
{
	Vector3 floorScale = Vector3(30.0f, 0.5f, 30.0f);
	Vector4 floorColour = Vector4(0.74f, 0.76f, 0.76f, 1.0f);
	AddCubeToWorld(Vector3(20.0f, 0, 0), Vector3(0, 0, 0), floorScale, GetTexture("basic"), floorColour, 0.0f, true);
	AddCubeToWorld(Vector3(140.0f, 0, 0), Vector3(0, 0, 0), floorScale, GetTexture("basic"), floorColour, 0.0f, true);
	AddCubeToWorld(Vector3(80.0f, 0, 60.0f), Vector3(0, 0, 0), floorScale, GetTexture("basic"), floorColour, 0.0f, true);
	AddCubeToWorld(Vector3(80.0f, 0, -60.0f), Vector3(0, 0, 0), floorScale, GetTexture("basic"), floorColour, 0.0f, true);

	//AddConcaveObjectToWorld(GetMesh("floorMain"), Vector3(0, 60.0f, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), GetTexture("basic"), floorColour, 0.0f);
	//AddConcaveObjectToWorld(GetMesh("platformMain"), Vector3(0, 60.0f, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), GetTexture("basic"), floorColour, 0.0f);

	Vector3 wallScaleX = Vector3(0.5f, 20.0f, 30.0f);
	Vector3 wallScaleZ = Vector3(30.0f, 20.0f, 0.5f);
	Vector4 wallColour = Vector4(0.49f, 0.51f, 0.51f, 1.0f);
	AddCubeToWorld(Vector3(-10.0f, 20.0f, 0.0f), Vector3(0, 0, 0), wallScaleX, GetTexture("basic"), wallColour, 0.0f);
	AddCubeToWorld(Vector3(170.0f, 20.0f, 0.0f), Vector3(0, 0, 0), wallScaleX, GetTexture("basic"), wallColour, 0.0f);
	
	AddCubeToWorld(Vector3(80.0f, 20.0f, 90.0f), Vector3(0, 0, 0), wallScaleZ, GetTexture("basic"), wallColour, 0.0f);
	AddCubeToWorld(Vector3(80.0f, 20.0f, -90.0f), Vector3(0, 0, 0), wallScaleZ, GetTexture("basic"),wallColour, 0.0f);

	Vector3 wallSmallScaleX = Vector3(0.5f, 10.0f, 30.0f);
	Vector3 wallSmallScaleZ = Vector3(30.0f, 10.0f, 0.5f);
	AddCubeToWorld(Vector3(20.0f, 10.0f, -30.0f), Vector3(0, 0, 0), wallSmallScaleZ, GetTexture("basic"), wallColour, 0.0f);
	AddCubeToWorld(Vector3(20.0f, 10.0f, 30.0f), Vector3(0, 0, 0), wallSmallScaleZ, GetTexture("basic"), wallColour, 0.0f);
	AddCubeToWorld(Vector3(140.0f, 10.0f, -30.0f), Vector3(0, 0, 0), wallSmallScaleZ, GetTexture("basic"), wallColour, 0.0f);
	AddCubeToWorld(Vector3(140.0f, 10.0f, 30.0f), Vector3(0, 0, 0), wallSmallScaleZ, GetTexture("basic"), wallColour, 0.0f);

	AddCubeToWorld(Vector3(110.0f, 10.0f, 60.0f), Vector3(0, 0, 0), wallSmallScaleX, GetTexture("basic"), wallColour, 0.0f);
	AddCubeToWorld(Vector3(50.0f, 10.0f, 60.0f), Vector3(0, 0, 0), wallSmallScaleX, GetTexture("basic"), wallColour, 0.0f);
	AddCubeToWorld(Vector3(50.0f, 10.0f, -60.0f), Vector3(0, 0, 0), wallSmallScaleX, GetTexture("basic"), wallColour, 0.0f);
	AddCubeToWorld(Vector3(110.0f, 10.0f, -60.0f), Vector3(0, 0, 0), wallSmallScaleX, GetTexture("basic"), wallColour, 0.0f);

	Vector3 bridgeScaleX = Vector3(30.0f, 0.5f, 5.0f);
	Vector3 bridgeScaleZ = Vector3(5.0f, 0.5f, 30.0f);
	Vector4 bridgeColour = Vector4(0.83f, 0.34f, 0.36f, 1.0f);
	AddCubeToWorld(Vector3(80.0f, 0.0f, 0.0f), Vector3(0, 0, 0), bridgeScaleX, GetTexture("basic"), bridgeColour, 0.0f, true);
	AddCubeToWorld(Vector3(80.0f, 0.5f, 0.0f), Vector3(0, 0, 0), bridgeScaleZ, GetTexture("basic"), bridgeColour, 0.0f, true);

	Vector3 containerScaleSmall = Vector3(3.0f, 3.0f, 3.0f);
	Vector3 containerScaleTall = Vector3(3.0f, 6.0f, 3.0f);
	Vector3 containerScaleBig = Vector3(10.0f, 6.0f, 4.0f);
	Vector4 boxColour = Vector4(0.98f, 0.95f, 0.79f, 1.0f);
	AddCubeToWorld(Vector3(30.0f, containerScaleSmall.y + 0.5f, -15.0f), Vector3(0, 0, 0), containerScaleSmall, GetTexture("basicPurple"), boxColour, 0.0f);
	AddCubeToWorld(Vector3(20.0f, containerScaleSmall.y + 0.5f, 15.0f), Vector3(0, 0, 0), containerScaleSmall, GetTexture("basicPurple"), boxColour, 0.0f);
	AddCubeToWorld(Vector3(5.0f, containerScaleSmall.y + 0.5f, 0.0f), Vector3(0, 0, 0), containerScaleSmall, GetTexture("basicPurple"), boxColour, 0.0f);

	AddCubeToWorld(Vector3(160.0f, containerScaleSmall.y + 0.5f, 20.0f), Vector3(0, 0, 0), containerScaleSmall, GetTexture("basicPurple"), boxColour, 0.0f);
	AddCubeToWorld(Vector3(125.0f, containerScaleTall.y + 0.5f, -15.0f), Vector3(0, 0, 0), containerScaleTall, GetTexture("basicPurple"), boxColour, 0.0f);
	
	AddCubeToWorld(Vector3(73.0f, containerScaleTall.y + 0.5f, -45.0f), Vector3(0, 0, 0), containerScaleTall, GetTexture("basicPurple"), boxColour, 0.0f);
	AddCubeToWorld(Vector3(80.0f, containerScaleSmall.y + 0.5f, -45.0f), Vector3(0, 0, 0), containerScaleSmall, GetTexture("basicPurple"), boxColour, 0.0f);
	AddCubeToWorld(Vector3(87.0f, containerScaleTall.y + 0.5f, -45.0f), Vector3(0, 0, 0), containerScaleTall, GetTexture("basicPurple"), boxColour, 0.0f);

	AddCubeToWorld(Vector3(80.0f, containerScaleBig.y + 0.5f, 60.0f), Vector3(0, 0, 0), containerScaleBig, GetTexture("basicPurple"), Debug::BLACK, 0.0f);

	axisObject = AddCubeToWorld(Vector3(40.0f, 10.0f, -20.0f), Vector3(0, 0, 0), Vector3(4, 1, 1), GetTexture("basicPurple"), Debug::WHITE, 1.0f);
	axisObject->GetRigidbody()->setType(reactphysics3d::BodyType::DYNAMIC);
	if (networkObjectList) {
		ToonNetworkObject* netO = new ToonNetworkObject(axisObject, axisObject->GetWorldID());
		networkObjectList->push_back(netO);
	}
	axisObject = AddCubeToWorld(Vector3(40.0f, 10.0f, 20.0f), Vector3(0, 0, 0), Vector3(4, 1, 1), GetTexture("basicPurple"), Debug::WHITE, 1.0f);
	axisObject->GetRigidbody()->setType(reactphysics3d::BodyType::DYNAMIC);
	if (networkObjectList) {
		ToonNetworkObject* netO = new ToonNetworkObject(axisObject, axisObject->GetWorldID());
		networkObjectList->push_back(netO);
	}

	return true;
}

reactphysics3d::ConcaveMeshShape* NCL::CSC8503::ToonLevelManager::CreateConcaveMeshShape(MeshGeometry* mesh, const Vector3& scaling)
{
	reactphysics3d::ConcaveMeshShape* concaveShape;

	unsigned int meshVertCount = mesh->GetVertexCount();
	unsigned int meshIndicesCount = mesh->GetIndexCount();
	unsigned int meshTrianglesCount = mesh->GetIndexCount() / 3;

	const void* meshVertStart = mesh->GetPositionData().data();
	const void* meshIndexStart = mesh->GetIndexData().data();

	reactphysics3d::TriangleVertexArray* vertexArray = new reactphysics3d::TriangleVertexArray(meshVertCount, meshVertStart, sizeof(NCL::Maths::Vector3),
																							   meshTrianglesCount, meshIndexStart, 3 * sizeof(int), 
																							   rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
																							   rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

	reactphysics3d::TriangleMesh* triangleMesh = gameWorld->GetPhysicsCommon().createTriangleMesh();
	triangleMesh->addSubpart(vertexArray);

	concaveShape = gameWorld->GetPhysicsCommon().createConcaveMeshShape(triangleMesh, ToonUtils::ConvertToRP3DVector3(scaling));

	return concaveShape;
}


PaintableObject* NCL::CSC8503::ToonLevelManager::AddCubeToWorld(const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, TextureBase* cubeTex, Vector4 minimapColour, float mass, bool isFloor)
{
	PaintableObject* cube = new PaintableObject(gameWorld->GetPhysicsWorld(), gameWorld);

	if (isFloor) cube->SetAsFloor();

	cube->GetTransform().SetPosition(position).
		SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z)).
		SetScale(scale * 2.0f);

	//std::cout << cube->GetTransform().GetMatrix() << std::endl;

	cube->AddRigidbody();
	cube->GetRigidbody()->setType(reactphysics3d::BodyType::STATIC);
	cube->GetRigidbody()->setMass(mass);
	cube->GetRigidbody()->setIsAllowedToSleep(true);
	cube->SetRenderObject(new ToonRenderObject(&cube->GetTransform(), GetMesh("cube"), cubeTex, GetShader("basic")));
  cube->GetRenderObject()->SetColour(minimapColour);

	const reactphysics3d::Vector3 boxExtent(scale.x, scale.y, scale.z);
	reactphysics3d::BoxShape* cubeBoxShape = gameWorld->GetPhysicsCommon().createBoxShape(boxExtent);
	cube->SetCollisionShape(cubeBoxShape);

	//reactphysics3d::Collider* cubeCollider = cube->GetRigidbody()->addCollider(cubeBoxShape, reactphysics3d::Transform::identity());

	cube->SetCollider(cubeBoxShape);
	cube->GetCollider()->getMaterial().setBounciness(0.1f);

	cube->GetRigidbody()->setUserData(cube);

	gameWorld->AddGameObject(cube);
	gameWorld->AddPaintableObject(cube);


	return cube;
}

PaintableObject* NCL::CSC8503::ToonLevelManager::AddSphereToWorld(const Vector3& position, const Vector3& rotationEuler, const float& radius, TextureBase* sphereTex, Vector4 minimapColour, float mass)
{
	PaintableObject* sphere = new PaintableObject(gameWorld->GetPhysicsWorld(), gameWorld);

	sphere->GetTransform().SetPosition(position).
		SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z)).
		SetScale(Vector3(radius, radius, radius));

	//std::cout << cube->GetTransform().GetMatrix() << std::endl;

	sphere->AddRigidbody();
	sphere->GetRigidbody()->setType(reactphysics3d::BodyType::STATIC);
	sphere->GetRigidbody()->setIsAllowedToSleep(true);
	sphere->SetRenderObject(new ToonRenderObject(&sphere->GetTransform(), GetMesh("sphere"), sphereTex, GetShader("basic")));
  sphere->GetRenderObject()->SetColour(minimapColour);
  
	reactphysics3d::SphereShape* sphereShape = gameWorld->GetPhysicsCommon().createSphereShape(radius * 0.85f);
	sphere->SetCollisionShape(sphereShape);

	//reactphysics3d::Collider* cubeCollider = cube->GetRigidbody()->addCollider(cubeBoxShape, reactphysics3d::Transform::identity());

	sphere->SetCollider(sphereShape);
	sphere->GetCollider()->getMaterial().setBounciness(0.1f);

	gameWorld->AddGameObject(sphere);

	return sphere;
}

void NCL::CSC8503::ToonLevelManager::AddGridWorld(Axes axes, const Vector3& gridSize, const float& gridSpacing, const Vector3& gridPosition, const Vector3& cubeScale, const float& cubeMass, TextureBase* cubeTex, Vector4 minimapColour, bool isFloor)
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

				AddCubeToWorld(newPos, Vector3(0, 0, 0), cubeScale, cubeTex, minimapColour, isFloor, 0.0f);
			}
		}
	}
}

PaintableObject* NCL::CSC8503::ToonLevelManager::AddConcaveObjectToWorld(MeshGeometry* mesh, const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, TextureBase* cubeTex, Vector4 minimapColour, float mass)
{
	PaintableObject* gameObject = new PaintableObject(gameWorld->GetPhysicsWorld(), gameWorld);
	gameObject->GetTransform().SetPosition(position).
		SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z)).
		SetScale(scale);

	gameObject->AddRigidbody();
	gameObject->GetRigidbody()->setType(reactphysics3d::BodyType::STATIC);
	gameObject->GetRigidbody()->setMass(mass);
	gameObject->GetRigidbody()->setIsAllowedToSleep(true);
	gameObject->SetRenderObject(new ToonRenderObject(&gameObject->GetTransform(), mesh, cubeTex, GetShader("basic")));
	gameObject->GetRenderObject()->SetColour(minimapColour);

	reactphysics3d::ConcaveMeshShape* concaveShape = CreateConcaveMeshShape(mesh, scale);

	gameObject->SetCollisionShape(concaveShape);
	gameObject->SetCollider(concaveShape);
	gameObject->GetCollider()->getMaterial().setBounciness(0.1f);

	gameObject->GetRigidbody()->setUserData(gameObject);

	gameWorld->AddGameObject(gameObject);
	gameWorld->AddPaintableObject(gameObject);

	return gameObject;
}

Player* ToonLevelManager::AddPlayerToWorld(const Vector3& position, Team* team) 
{
	const float PLAYER_RADIUS = 2.0f;
	const float PLAYER_HEIGHT = 0.38f;
	player = new Player(gameWorld->GetPhysicsWorld(), gameWorld, team);
	player->AddRigidbody();

	player->SetPosition(position);
	player->GetTransform().SetScale(Vector3(PLAYER_RADIUS * 1.1f, PLAYER_RADIUS * 1.1f, PLAYER_RADIUS * 1.1f));

	player->GetRigidbody()->setType(reactphysics3d::BodyType::DYNAMIC);
	player->GetRigidbody()->setLinearDamping(0.8f);
	player->GetRigidbody()->setAngularLockAxisFactor(reactphysics3d::Vector3(0, 0, 0));
	player->GetRigidbody()->setIsAllowedToSleep(true);

	//reactphysics3d::SphereShape* sphereShape = gameWorld->GetPhysicsCommon().createSphereShape(PLAYER_RADIUS * 0.85f);
	reactphysics3d::CapsuleShape* capsuleShape = gameWorld->GetPhysicsCommon().createCapsuleShape(PLAYER_RADIUS, PLAYER_HEIGHT);

	reactphysics3d::Transform capsuleTransform(ToonUtils::ConvertToRP3DVector3(Vector3(0, 2.2f, 0)), reactphysics3d::Quaternion::identity());
	player->SetCollisionShape(capsuleShape);
	player->SetCollider(capsuleShape, capsuleTransform);
	player->SetColliderLayer(ToonCollisionLayer::Character);

	int collisionMask = ToonCollisionLayer::Character | ToonCollisionLayer::Default;
	player->SetColliderLayerMask(ToonCollisionLayer(collisionMask));

	player->GetCollider()->getMaterial().setBounciness(0.1f);

	player->GetRigidbody()->setUserData(player);

	gameWorld->AddGameObject(player);
	player->SetRenderObject(new ToonRenderObject(&player->GetTransform(), GetMesh("player"), GetTexture("boss_player"), GetShader("animated"), GetMesh("arrow")));
	player->GetRenderObject()->SetColour(Vector4(team->GetTeamColour(), 1));

	return player;
}

PaintBallProjectile* ToonLevelManager::AddPaintBallProjectileToWorld(const reactphysics3d::Vector3& position,
	const reactphysics3d::Vector3& rotationEuler, const float& radius, const float& _impactSize, Team* team) {
	PaintBallProjectile* paintball = new PaintBallProjectile(gameWorld->GetPhysicsWorld(), gameWorld, _impactSize, team);
	paintball->AddRigidbody();
	paintball->SetPosition(position);
	paintball->SetOrientation(rotationEuler);
	paintball->GetTransform().SetScale(Vector3(radius, radius, radius));

	paintball->SetRenderObject(new ToonRenderObject(&paintball->GetTransform(), GetMesh("sphere"), GetTexture("basic"), GetShader("basic")));
	paintball->GetRenderObject()->SetColour(Vector4(team->GetTeamColour(), 1.0f));

	paintball->GetRigidbody()->setType(reactphysics3d::BodyType::DYNAMIC);
	paintball->GetRigidbody()->setMass(reactphysics3d::decimal(0.1));

	reactphysics3d::SphereShape* sphereShape = gameWorld->GetPhysicsCommon().createSphereShape(radius);
	paintball->SetCollisionShape(sphereShape);
	paintball->SetCollider(sphereShape);
	paintball->GetCollider()->getMaterial().setBounciness(0.1f);

	paintball->GetRigidbody()->setUserData(paintball);

	gameWorld->AddGameObject(paintball);
	gameWorld->AddPaintball(paintball);
	return paintball;
}

HitSphere* ToonLevelManager::AddHitSphereToWorld(const reactphysics3d::Vector3& position, const float radius, Team* team) {
	HitSphere* hitSphere = new HitSphere(gameWorld->GetPhysicsWorld(), gameWorld, team, radius);
	hitSphere->AddRigidbody();
	hitSphere->SetPosition(position);
	hitSphere->GetTransform().SetScale(Vector3(radius, radius, radius));

	hitSphere->SetRenderObject(new ToonRenderObject(&hitSphere->GetTransform(), GetMesh("sphere"), GetTexture("basic"), GetShader("basic")));
	hitSphere->GetRenderObject()->SetColour(Vector4(team->GetTeamColour(), 0.0f));

	hitSphere->GetRigidbody()->setType(reactphysics3d::BodyType::DYNAMIC);

	reactphysics3d::SphereShape* sphereShape = gameWorld->GetPhysicsCommon().createSphereShape(radius);
	hitSphere->SetCollisionShape(sphereShape);
	hitSphere->SetCollider(sphereShape);
	hitSphere->GetCollider()->getMaterial().setBounciness(0.1f);

	//GetCollider()->setIsTrigger(true);

	hitSphere->GetRigidbody()->setUserData(hitSphere);

	gameWorld->AddGameObject(hitSphere);
	gameWorld->AddHitSphere(hitSphere);
	return hitSphere;
}