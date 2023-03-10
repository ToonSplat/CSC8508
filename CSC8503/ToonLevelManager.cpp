#include "ToonRenderObject.h"
#include "ToonGameObject.h"
#include "ToonLevelManager.h"
#include "ToonGameWorld.h"
#include <reactphysics3d/reactphysics3d.h>
#include "Maths.h"

using namespace NCL;
using namespace CSC8503;

NCL::CSC8503::ToonLevelManager::ToonLevelManager(ToonGameWorld* gameWorld) : gameWorld(gameWorld){
	if (!LoadAssets()) return;
}

NCL::CSC8503::ToonLevelManager::~ToonLevelManager() {
}

void NCL::CSC8503::ToonLevelManager::Update(float dt)
{
	
}

bool NCL::CSC8503::ToonLevelManager::LoadAssets()
{
	//All Models
	if (!LoadModel("cube")) return false;
	if (!LoadModel("arrow")) return false;
	if (!LoadModel("player")) return false;
	if (!LoadModel("player_mesh_1")) return false;
	if (!LoadModel("player_mesh_2")) return false;
	if (!LoadModel("player_mesh_3")) return false;
	if (!LoadModel("player_mesh_4")) return false;
	if (!LoadModel("sphere")) return false;

	//if (!LoadModel("arena_main")) return false;
	if (!LoadModel("arena_floor_part_a")) return false;
	if (!LoadModel("arena_floor_part_b")) return false;
	if (!LoadModel("arena_floor_part_c")) return false;
	if (!LoadModel("arena_floor_part_d")) return false;

	if (!LoadModel("arena_wall_part_a")) return false;
	if (!LoadModel("arena_wall_part_b")) return false;
	if (!LoadModel("arena_wall_part_c")) return false;
	if (!LoadModel("arena_wall_part_d")) return false;

	if (!LoadModel("arena_platform_floor_high_l")) return false;
	if (!LoadModel("arena_platform_floor_high_r")) return false;
	if (!LoadModel("arena_platform_wall_high_l")) return false;
	if (!LoadModel("arena_platform_wall_high_r")) return false;

	if (!LoadModel("arena_platform_floor_low_top")) return false;
	if (!LoadModel("arena_platform_floor_low_bottom")) return false;
	if (!LoadModel("arena_platform_wall_low_top")) return false;
	if (!LoadModel("arena_platform_wall_low_bottom")) return false;


	if (!LoadModel("arena_lights")) return false;
	
	if (!LoadModel("arena_deco_cats")) return false;
	if (!LoadModel("arena_deco_cats_hull")) return false;

	if (!LoadModel("arena_deco_chair")) return false;
	if (!LoadModel("arena_deco_chair_throne")) return false;
	if (!LoadModel("arena_deco_table")) return false;
	if (!LoadModel("arena_deco_screen")) return false;

	if (!LoadModel("arena_obstacles")) return false;
	if (!LoadModel("arena_obstacles_hulls")) return false;
	if (!LoadModel("arena_ramps")) return false;
	if (!LoadModel("arena_decos")) return false;
	if (!LoadModel("arena_decos_hulls")) return false;
	if (!LoadModel("arena_border_wall")) return false;
	if (!LoadModel("arena_cameras")) return false;
	if (!LoadModel("arena_cameras_hulls")) return false;
	if (!LoadModel("arena_toonsplat_Box")) return false;
	//if (!LoadModel("floorMain")) return false;
	//if (!LoadModel("platformMain")) return false;

	//All Textures
	if (!LoadTexture("mesh")) return false;
	if (!LoadTexture("basic")) return false;
	if (!LoadTexture("basicPurple")) return false;
	/*if (!LoadTexture("player")) return false;
	if (!LoadTexture("tex_arena_wall")) return false;
	if (!LoadTexture("tex_arena_wall2")) return false;
	if (!LoadTexture("tex_arena_lights")) return false;*/

	//All Shaders
	if (!LoadShader("scene")) return false;
	if (!LoadShader("animated")) return false;
	if (!LoadShader("sceneScreen")) return false;

	//All Materials
	if (!LoadMaterial("mat_player")) return false;

	//if (!LoadMaterial("mat_arena")) return false;
	if (!LoadMaterial("mat_arena_floor_part_a")) return false;
	if (!LoadMaterial("mat_arena_floor_part_b")) return false;
	if (!LoadMaterial("mat_arena_floor_part_c")) return false;
	if (!LoadMaterial("mat_arena_floor_part_d")) return false;

	if (!LoadMaterial("mat_arena_wall_part_a")) return false;
	if (!LoadMaterial("mat_arena_wall_part_b")) return false;
	if (!LoadMaterial("mat_arena_wall_part_c")) return false;
	if (!LoadMaterial("mat_arena_wall_part_d")) return false;

	if (!LoadMaterial("mat_arena_platform_floor_high_l")) return false;
	if (!LoadMaterial("mat_arena_platform_floor_high_r")) return false;
	if (!LoadMaterial("mat_arena_platform_wall_high_l")) return false;
	if (!LoadMaterial("mat_arena_platform_wall_high_r")) return false;

	if (!LoadMaterial("mat_arena_platform_floor_low_top")) return false;
	if (!LoadMaterial("mat_arena_platform_floor_low_bottom")) return false;
	if (!LoadMaterial("mat_arena_platform_wall_low_top")) return false;
	if (!LoadMaterial("mat_arena_platform_wall_low_bottom")) return false;

	if (!LoadMaterial("mat_arena_obstacles")) return false;
	if (!LoadMaterial("mat_arena_obstacles_hulls")) return false;
	if (!LoadMaterial("mat_arena_ramps")) return false;
	if (!LoadMaterial("mat_arena_lights")) return false;

	if (!LoadMaterial("mat_arena_deco_cats")) return false;
	if (!LoadMaterial("mat_arena_deco_cats_hull")) return false;

	if (!LoadMaterial("mat_arena_deco_chair")) return false;
	if (!LoadMaterial("mat_arena_deco_chair_throne")) return false;
	if (!LoadMaterial("mat_arena_deco_table")) return false;
	if (!LoadMaterial("mat_arena_deco_screen")) return false;
	
	if (!LoadMaterial("mat_arena_decos")) return false;
	if (!LoadMaterial("mat_arena_decos_hulls")) return false;
	if (!LoadMaterial("mat_arena_border_wall")) return false;
	if (!LoadMaterial("mat_arena_cameras")) return false;
	if (!LoadMaterial("mat_arena_cameras_hulls")) return false;
	if (!LoadMaterial("mat_arena_toonsplat_Box")) return false;

	std::cout << "ToonLevelManager: All files successfully loaded\n";

	return true;
}

bool NCL::CSC8503::ToonLevelManager::LoadModel(const std::string meshName)
{
	meshMap[meshName] = ToonAssetManager::Instance().GetMesh(meshName);
	return(meshMap[meshName] != nullptr);
}

bool NCL::CSC8503::ToonLevelManager::LoadTexture(const std::string textureName)
{
	textureMap[textureName] = ToonAssetManager::Instance().GetTexture(textureName);
	return(textureMap[textureName] != nullptr);
}

bool NCL::CSC8503::ToonLevelManager::LoadShader(const std::string shaderName)
{
	shaderMap[shaderName] = ToonAssetManager::Instance().GetShader(shaderName);
	return(shaderMap[shaderName] != nullptr);
}

bool NCL::CSC8503::ToonLevelManager::LoadMaterial(const std::string materialName)
{
	materialMap[materialName] = ToonAssetManager::Instance().GetMaterial(materialName);
	return(materialMap[materialName] != nullptr);
}

void ToonLevelManager::ResetLevel(std::vector<ToonNetworkObject*>* networkObjectList) {
	gameWorld->ClearAndErase();
	//LoadPrototypeLevel(networkObjectList);
	LoadArenaLevel(networkObjectList);
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

bool NCL::CSC8503::ToonLevelManager::LoadArenaLevel(std::vector<ToonNetworkObject*>* networkObjectList)
{
	Vector4 arenaColour = Vector4(0.76f, 0.76f, 0.76f, 1.0f);
	Vector4 arenaDecosColour = Vector4(0.65f, 0.65f, 0.65f, 1.0f);
	Vector4 arenaObstaclesColor = Vector4(0.9f, 0.9f, 0.0f, 1.0f);
	Vector4 arenaRampColor = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	Vector4 arenaLightsColor = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	Vector3 arenaSize = Vector3(1.0f, 1.0f, 1.0f);
	//AddConcaveObjectToWorld(GetMesh("arena_main"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena"), arenaColour, 0.0f);
	AddConcaveObjectToWorld(GetMesh("arena_floor_part_a"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_floor_part_a"), arenaColour, 0.0f, true, true);
	AddConcaveObjectToWorld(GetMesh("arena_floor_part_b"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_floor_part_b"), arenaColour, 0.0f, true, true);
	AddConcaveObjectToWorld(GetMesh("arena_floor_part_c"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_floor_part_c"), arenaColour, 0.0f, true, true);
	AddConcaveObjectToWorld(GetMesh("arena_floor_part_d"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_floor_part_d"), arenaColour, 0.0f, true, true);
	
	AddConcaveObjectToWorld(GetMesh("arena_wall_part_a"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_wall_part_a"), arenaColour, 0.0f, true, false);
	AddConcaveObjectToWorld(GetMesh("arena_wall_part_b"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_wall_part_b"), arenaColour, 0.0f, true, false);
	AddConcaveObjectToWorld(GetMesh("arena_wall_part_c"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_wall_part_c"), arenaColour, 0.0f, true, false);
	AddConcaveObjectToWorld(GetMesh("arena_wall_part_d"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_wall_part_d"), arenaColour, 0.0f, true, false);
	
	AddConcaveObjectToWorld(GetMesh("arena_platform_floor_high_l"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_platform_floor_high_l"), arenaColour, 0.0f, true, true);
	AddConcaveObjectToWorld(GetMesh("arena_platform_floor_high_r"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_platform_floor_high_r"), arenaColour, 0.0f, true, true);
	AddConcaveObjectToWorld(GetMesh("arena_platform_wall_high_l"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_platform_wall_high_l"), arenaColour, 0.0f, true, false);
	AddConcaveObjectToWorld(GetMesh("arena_platform_wall_high_r"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_platform_wall_high_r"), arenaColour, 0.0f, true, false);
	
	AddConcaveObjectToWorld(GetMesh("arena_platform_floor_low_top"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_platform_floor_low_top"), arenaColour, 0.0f, true, true);
	AddConcaveObjectToWorld(GetMesh("arena_platform_floor_low_bottom"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_platform_floor_low_bottom"), arenaColour, 0.0f, true, true);
	AddConcaveObjectToWorld(GetMesh("arena_platform_wall_low_top"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_platform_wall_low_top"), arenaColour, 0.0f, true, false);
	AddConcaveObjectToWorld(GetMesh("arena_platform_wall_low_bottom"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_platform_wall_low_bottom"), arenaColour, 0.0f, true, false);
	
	AddConcaveObjectToWorld(GetMesh("arena_obstacles"), GetMesh("arena_obstacles_hulls"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_obstacles"), arenaObstaclesColor, 0.0f, true, false);
	AddConcaveObjectToWorld(GetMesh("arena_ramps"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_ramps"), arenaRampColor, 0.0f, true, true);
	AddConcaveObjectToWorld(GetMesh("arena_lights"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_lights"), arenaLightsColor, 0.0f, true, false);
	AddConcaveObjectToWorld(GetMesh("arena_decos"), GetMesh("arena_decos_hulls"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_decos"), arenaDecosColour, 0.0f, true, false);
	AddConcaveObjectToWorld(GetMesh("arena_cameras"), GetMesh("arena_cameras_hulls"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_cameras"), arenaDecosColour, 0.0f, true, false);
	AddConcaveObjectToWorld(GetMesh("arena_border_wall"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_border_wall"), arenaDecosColour, 0.0f, false, false);
	
	AddConcaveObjectToWorld(GetMesh("arena_deco_cats"), GetMesh("arena_deco_cats_hull"), Vector3(0, 0.0f, 0), Vector3(0, 0, 0), arenaSize, GetMaterial("mat_arena_deco_cats"), arenaLightsColor, 0.0f, true, false);
	
	//Those Boxes with ToonSplat logos on it
	BoxCollisionShapeData* boxData = new BoxCollisionShapeData(Vector3(2.65f, 2.65f, 2.65f));
	AddPropObject(GetMesh("arena_toonsplat_Box"), boxData, reactphysics3d::BodyType::DYNAMIC, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), GetMaterial("mat_arena_toonsplat_Box"), arenaDecosColour, 0.1f, true, false, networkObjectList);
	AddPropObject(GetMesh("arena_toonsplat_Box"), boxData, reactphysics3d::BodyType::DYNAMIC, Vector3(0, 7.0f, -45.0f), Vector3(0, 0, 0), Vector3(1, 1, 1), GetMaterial("mat_arena_toonsplat_Box"), arenaDecosColour, 0.1f, true, false, networkObjectList);
	AddPropObject(GetMesh("arena_toonsplat_Box"), boxData, reactphysics3d::BodyType::DYNAMIC, Vector3(0, 7.0f, 45.0f), Vector3(0, 0, 0), Vector3(1, 1, 1), GetMaterial("mat_arena_toonsplat_Box"), arenaDecosColour, 0.1f, true, false, networkObjectList);

	BoxCollisionShapeData* chairData = new BoxCollisionShapeData(Vector3(1.5f, 2.35f, 1.0f), reactphysics3d::Transform(ToonUtils::ConvertToRP3DVector3(Vector3(0.0f, 0.25f, 0.0f)), reactphysics3d::Quaternion::identity()));
	AddPropObject(GetMesh("arena_deco_chair"), chairData, reactphysics3d::BodyType::DYNAMIC, Vector3(-50.0f, 16.0f, -5.0f), Vector3(0, 0, 0), Vector3(1, 1, 1), GetMaterial("mat_arena_deco_chair"), arenaDecosColour, 0.1f, true, false, networkObjectList);
	AddPropObject(GetMesh("arena_deco_chair"), chairData, reactphysics3d::BodyType::DYNAMIC, Vector3(-50.0f, 16.0f, 5.0f), Vector3(0, 0, 0), Vector3(1, 1, 1), GetMaterial("mat_arena_deco_chair"), arenaDecosColour, 0.1f, true, false, networkObjectList);
	AddPropObject(GetMesh("arena_deco_chair"), chairData, reactphysics3d::BodyType::DYNAMIC, Vector3(50.0f, 16.0f, -5.0f), Vector3(0, 3.14f, 0), Vector3(1, 1, 1), GetMaterial("mat_arena_deco_chair"), arenaDecosColour, 0.1f, true, false, networkObjectList);
	AddPropObject(GetMesh("arena_deco_chair"), chairData, reactphysics3d::BodyType::DYNAMIC, Vector3(50.0f, 16.0f, 5.0f), Vector3(0, 3.14f, 0), Vector3(1, 1, 1), GetMaterial("mat_arena_deco_chair"), arenaDecosColour, 0.1f, true, false, networkObjectList);
	
	BoxCollisionShapeData* throneChairData = new BoxCollisionShapeData(Vector3(2.0f, 4.3f, 2.0f), reactphysics3d::Transform(ToonUtils::ConvertToRP3DVector3(Vector3(0.0f, 2.25f, 0.0f)), reactphysics3d::Quaternion::identity()));
	AddPropObject(GetMesh("arena_deco_chair_throne"), throneChairData, reactphysics3d::BodyType::DYNAMIC, Vector3(50.0f, 16.0f, 0.0f), Vector3(0, 3.14f, 0), Vector3(1, 1, 1), GetMaterial("mat_arena_deco_chair_throne"), arenaDecosColour, 0.1f, true, false, networkObjectList);
	AddPropObject(GetMesh("arena_deco_chair_throne"), throneChairData, reactphysics3d::BodyType::DYNAMIC, Vector3(-50.0f, 16.0f, 0.0f), Vector3(0, 0.0f, 0), Vector3(1, 1, 1), GetMaterial("mat_arena_deco_chair_throne"), arenaDecosColour, 0.1f, true, false, networkObjectList);
	
	BoxCollisionShapeData* tableData = new BoxCollisionShapeData(Vector3(5.0f, 1.5f, 3.0f), reactphysics3d::Transform(ToonUtils::ConvertToRP3DVector3(Vector3(0.0f, 1.5f, 3.0f)), reactphysics3d::Quaternion::identity()));
	AddPropObject(GetMesh("arena_deco_table"), tableData, reactphysics3d::BodyType::DYNAMIC, Vector3(32.0f, -4.0f, -32.0f), Vector3(0.0f, -45.0f, 0.0f), Vector3(3.0f, 3.0f, 3.0f), GetMaterial("mat_arena_deco_table"), arenaDecosColour, 0.1f, true, false, networkObjectList);
	AddPropObject(GetMesh("arena_deco_table"), tableData, reactphysics3d::BodyType::DYNAMIC, Vector3(-32.0f, -4.0f, -32.0f), Vector3(0.0f, 45.0f, 0.0f), Vector3(3.0f, 3.0f, 3.0f), GetMaterial("mat_arena_deco_table"), arenaDecosColour, 0.1f, true, false, networkObjectList);
	AddPropObject(GetMesh("arena_deco_table"), tableData, reactphysics3d::BodyType::DYNAMIC, Vector3(-32.0f, -4.0f, 32.0f), Vector3(0.0f, -180.0f, 0.0f), Vector3(3.0f, 3.0f, 3.0f), GetMaterial("mat_arena_deco_table"), arenaDecosColour, 0.1f, true, false, networkObjectList);
	AddPropObject(GetMesh("arena_deco_table"), tableData, reactphysics3d::BodyType::DYNAMIC, Vector3(32.0f, -4.0f, 32.0f), Vector3(0.0f, 180.0f, 0.0f), Vector3(3.0f, 3.0f, 3.0f), GetMaterial("mat_arena_deco_table"), arenaDecosColour, 0.1f, true, false, networkObjectList);
	
	BoxCollisionShapeData* screenData = new BoxCollisionShapeData(Vector3(12.0f, 5.6f, 1.5f), reactphysics3d::Transform(ToonUtils::ConvertToRP3DVector3(Vector3(0.0f, 0.0f, 0.0f)), reactphysics3d::Quaternion::identity()));
	AddPropScreen(GetMesh("arena_deco_screen"), screenData, reactphysics3d::BodyType::STATIC, Vector3(0.0f, 15.0f, -52.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), GetMaterial("mat_arena_deco_screen"), arenaDecosColour, 0.1f, true, false);
	AddPropScreen(GetMesh("arena_deco_screen"), screenData, reactphysics3d::BodyType::STATIC, Vector3(0.0f, 15.0f, 52.0f), Vector3(0.0f, 3.14f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), GetMaterial("mat_arena_deco_screen"), arenaDecosColour, 0.1f, true, false);

	//Delete these "now" useless data to avoid leaks
	delete boxData;
	delete chairData;
	delete throneChairData;
	delete screenData;

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
																							   reactphysics3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
																							   reactphysics3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

	reactphysics3d::TriangleMesh* triangleMesh = gameWorld->GetPhysicsCommon().createTriangleMesh();
	triangleMesh->addSubpart(vertexArray);

	concaveShape = gameWorld->GetPhysicsCommon().createConcaveMeshShape(triangleMesh, ToonUtils::ConvertToRP3DVector3(scaling));

	return concaveShape;
}

reactphysics3d::ConvexMeshShape* NCL::CSC8503::ToonLevelManager::CreateConvexMeshShape(MeshGeometry* mesh, const Vector3& scaling)
{
	reactphysics3d::ConvexMeshShape* convexShape;

	unsigned int meshVertCount = mesh->GetVertexCount();
	unsigned int meshIndicesCount = mesh->GetIndexCount();
	unsigned int meshTrianglesCount = mesh->GetIndexCount() / 3;

	const std::vector<Vector3> meshVertices = mesh->GetPositionData();
	const std::vector<unsigned int> meshIndices = mesh->GetIndexData();

	const void* meshVertStart = mesh->GetPositionData().data();
	const void* meshIndexStart = mesh->GetIndexData().data();

	reactphysics3d::PolygonVertexArray::PolygonFace* mPolygonFaces = new rp3d::PolygonVertexArray::PolygonFace[meshTrianglesCount];
	reactphysics3d::PolygonVertexArray::PolygonFace* face = mPolygonFaces;

	for (unsigned int f = 0; f < meshTrianglesCount; f++)
	{
		/*for (int v = 0; v < 3; v++) 
		{
			const Vector3 vertex = meshVertices[meshIndices[f * 3 + v]];
			int vIndex = mesh->FindVertexIndex(mConvexMeshVertices, vertex);
			if (vIndex == -1) {
				vIndex = mConvexMeshVertices.size();
				mConvexMeshVertices.push_back(vertex);
			}

			mConvexMeshIndices.push_back(vIndex);
		}*/

		face->indexBase = f * 3;
		face->nbVertices = 3;
		face++;
	}

	reactphysics3d::PolygonVertexArray* polygonVertexArray = new reactphysics3d::PolygonVertexArray(meshVertCount, meshVertStart, sizeof(NCL::Maths::Vector3),
																									meshIndexStart, sizeof(int), meshTrianglesCount, mPolygonFaces, 
																									reactphysics3d::PolygonVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
																									reactphysics3d::PolygonVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

	reactphysics3d::PolyhedronMesh* mPolyhedronMesh = gameWorld->GetPhysicsCommon().createPolyhedronMesh(polygonVertexArray);
	convexShape = gameWorld->GetPhysicsCommon().createConvexMeshShape(mPolyhedronMesh, ToonUtils::ConvertToRP3DVector3(scaling));

	return convexShape;
}

PaintableObject* NCL::CSC8503::ToonLevelManager::AddCubeToWorld(const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, TextureBase* cubeTex, Vector4 minimapColour, float mass, bool isFloor)
{
	PaintableObject* cube = new PaintableObject(gameWorld->GetPhysicsWorld(), gameWorld);

	if (isFloor) cube->SetAsFloor();

	cube->GetTransform().SetPosition(position).
		SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z)).
		SetScale(scale);

	//std::cout << cube->GetTransform().GetMatrix() << std::endl;

	cube->AddRigidbody();
	cube->GetRigidbody()->setType(reactphysics3d::BodyType::STATIC);
	cube->GetRigidbody()->setMass(mass);
	cube->GetRigidbody()->setIsAllowedToSleep(true);
	cube->SetRenderObject(new ToonRenderObject(&cube->GetTransform(), GetMesh("cube"), cubeTex, GetShader("scene")));
	cube->GetRenderObject()->SetMinimapColour(minimapColour);

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

PaintableObject* NCL::CSC8503::ToonLevelManager::AddTSCubeToWorld(const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, TextureBase* cubeTex, Vector4 minimapColour, float mass, bool isFloor)
{
	PaintableObject* tsCube = AddCubeToWorld(position, rotationEuler, scale, cubeTex, minimapColour, mass, isFloor);
	tsCube->GetRenderObject()->SetMesh(GetMesh("arena_toonsplat_Box"));
	tsCube->GetRigidbody()->setType(reactphysics3d::BodyType::DYNAMIC);
	tsCube->GetCollisionShapeBox()->setHalfExtents(reactphysics3d::Vector3(2.65f, 2.65f, 2.65f));
	tsCube->GetRenderObject()->SetMeshMaterial(GetMaterial("mat_arena_toonsplat_Box"));

	return tsCube;
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
	sphere->SetRenderObject(new ToonRenderObject(&sphere->GetTransform(), GetMesh("sphere"), sphereTex, GetShader("scene")));
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

PaintableObject* NCL::CSC8503::ToonLevelManager::AddConcaveObjectToWorld(MeshGeometry* mesh, const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, TextureBase* cubeTex, Vector4 minimapColour, float mass, float addAsPaintable, float addAsFloor)
{
	PaintableObject* gameObject = new PaintableObject(gameWorld->GetPhysicsWorld(), gameWorld);
	gameObject->GetTransform().SetPosition(position).
		SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z)).
		SetScale(scale);

	if (addAsFloor) gameObject->SetAsFloor();

	gameObject->AddRigidbody();
	gameObject->GetRigidbody()->setType(reactphysics3d::BodyType::STATIC);
	gameObject->GetRigidbody()->setMass(mass);
	gameObject->GetRigidbody()->setIsAllowedToSleep(true);
	gameObject->SetRenderObject(new ToonRenderObject(&gameObject->GetTransform(), mesh, cubeTex, GetShader("scene")));
	gameObject->GetRenderObject()->SetMinimapColour(minimapColour);

	reactphysics3d::ConcaveMeshShape* concaveShape = CreateConcaveMeshShape(mesh, scale);

	gameObject->SetCollisionShape(concaveShape);
	gameObject->SetCollider(concaveShape);
	gameObject->GetCollider()->getMaterial().setBounciness(0.1f);

	gameObject->GetRigidbody()->setUserData(gameObject);

	gameWorld->AddGameObject(gameObject);
	
	if(addAsPaintable)
		gameWorld->AddPaintableObject(gameObject);

	return gameObject;
}

PaintableObject* NCL::CSC8503::ToonLevelManager::AddConcaveObjectToWorld(MeshGeometry* mesh, const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, ToonMeshMaterial* mat, Vector4 minimapColour, float mass, float addAsPaintable, float addAsFloor)
{
	PaintableObject* gameObject = AddConcaveObjectToWorld(mesh, position, rotationEuler, scale, GetTexture("basic"), minimapColour, mass, addAsPaintable, addAsFloor);
	gameObject->GetRenderObject()->SetMeshMaterial(mat);	

	return gameObject;
}

PaintableObject* NCL::CSC8503::ToonLevelManager::AddConcaveObjectToWorld(MeshGeometry* mesh, MeshGeometry* collisionHullMesh, const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, ToonMeshMaterial* mat, Vector4 minimapColour, float mass, float addAsPaintable, float addAsFloor)
{
	PaintableObject* gameObject = new PaintableObject(gameWorld->GetPhysicsWorld(), gameWorld);
	gameObject->GetTransform().SetPosition(position).
		SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z)).
		SetScale(scale);

	if (addAsFloor) gameObject->SetAsFloor();

	gameObject->AddRigidbody();
	gameObject->GetRigidbody()->setType(reactphysics3d::BodyType::STATIC);
	gameObject->GetRigidbody()->setMass(mass);
	gameObject->GetRigidbody()->setIsAllowedToSleep(true);
	gameObject->SetRenderObject(new ToonRenderObject(&gameObject->GetTransform(), mesh, mat, GetShader("scene")));
	gameObject->GetRenderObject()->SetMinimapColour(minimapColour);

	reactphysics3d::ConcaveMeshShape* concaveShape = CreateConcaveMeshShape(collisionHullMesh, scale);

	gameObject->SetCollisionShape(concaveShape);
	gameObject->SetCollider(concaveShape);
	gameObject->GetCollider()->getMaterial().setBounciness(0.1f);

	gameObject->GetRigidbody()->setUserData(gameObject);

	gameWorld->AddGameObject(gameObject);

	if (addAsPaintable)
		gameWorld->AddPaintableObject(gameObject);

	return gameObject;
}

//PaintableObject* NCL::CSC8503::ToonLevelManager::AddDynamicConcaveObjectToWorld(MeshGeometry* mesh, MeshGeometry* collisionHullMesh, const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, ToonMeshMaterial* mat, Vector4 minimapColour, float mass, float addAsPaintable, float addAsFloor)
//{
//	PaintableObject* object = AddConcaveObjectToWorld(mesh, collisionHullMesh, position, rotationEuler, scale, mat, minimapColour, 0.1f, addAsPaintable, addAsFloor);
//	object->GetRigidbody()->setType(reactphysics3d::BodyType::DYNAMIC);	
//
//	return object;
//}

PaintableObject* NCL::CSC8503::ToonLevelManager::AddConvexObjectToWorld(MeshGeometry* mesh, const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, ToonMeshMaterial* mat, Vector4 minimapColour, float mass, float addAsPaintable, float addAsFloor)
{
	PaintableObject* gameObject = new PaintableObject(gameWorld->GetPhysicsWorld(), gameWorld);
	gameObject->GetTransform().SetPosition(position).
		SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z)).
		SetScale(scale);

	if (addAsFloor) gameObject->SetAsFloor();

	gameObject->AddRigidbody();
	gameObject->GetRigidbody()->setType(reactphysics3d::BodyType::STATIC);
	gameObject->GetRigidbody()->setMass(mass);
	gameObject->GetRigidbody()->setIsAllowedToSleep(true);
	gameObject->SetRenderObject(new ToonRenderObject(&gameObject->GetTransform(), mesh, mat, GetShader("scene")));
	gameObject->GetRenderObject()->SetMinimapColour(minimapColour);

	reactphysics3d::ConvexMeshShape* convexShape = CreateConvexMeshShape(mesh, scale);

	gameObject->SetCollisionShape(convexShape);
	gameObject->SetCollider(convexShape);
	gameObject->GetCollider()->getMaterial().setBounciness(0.1f);

	gameObject->GetRigidbody()->setUserData(gameObject);

	gameWorld->AddGameObject(gameObject);

	if (addAsPaintable)
		gameWorld->AddPaintableObject(gameObject);

	return gameObject;
}

PaintableObject* NCL::CSC8503::ToonLevelManager::AddPropObject(MeshGeometry* mesh, ObjectCollisionShapeData* collisionData, reactphysics3d::BodyType rigidbodyType, const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, ToonMeshMaterial* mat, Vector4 minimapColour, float mass, float addAsPaintable, float addAsFloor, std::vector<ToonNetworkObject*>* networkObjectList)
{
	PaintableObject* dynamicProp = new PaintableObject(gameWorld->GetPhysicsWorld(), gameWorld);
	dynamicProp->GetTransform().SetPosition(position).
		SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z)).
		SetScale(scale);

	//dynamicProp->SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z));

	dynamicProp->SetRenderObject(new ToonRenderObject(&dynamicProp->GetTransform(), mesh, mat, GetShader("scene")));
	dynamicProp->GetRenderObject()->SetMinimapColour(minimapColour);

	if (addAsFloor) dynamicProp->SetAsFloor();

	reactphysics3d::CollisionShape* collisionShape = nullptr;
	switch (collisionData->shapeType)
	{
		case BasicCollisionShapeType::Shape_Box:
		{
			BoxCollisionShapeData* boxData = static_cast<BoxCollisionShapeData*>(collisionData);
			collisionShape = gameWorld->GetPhysicsCommon().createBoxShape(ToonUtils::ConvertToRP3DVector3(boxData->boxHalfSize));
			break;
		}

		case BasicCollisionShapeType::Shape_Sphere:
		{
			SphereCollisionShapeData* sphereData = static_cast<SphereCollisionShapeData*>(collisionData);
			collisionShape = gameWorld->GetPhysicsCommon().createSphereShape(sphereData->radius);
			break;
		}

		case BasicCollisionShapeType::Shape_Capsule:
		{
			CapsuleCollisionShapeData* capsuleData = static_cast<CapsuleCollisionShapeData*>(collisionData);
			collisionShape = gameWorld->GetPhysicsCommon().createCapsuleShape(capsuleData->radius, capsuleData->height);
			break;
		}
	}

	dynamicProp->AddRigidbody();
	dynamicProp->GetRigidbody()->setType(rigidbodyType);
	dynamicProp->GetRigidbody()->setMass(mass);
	dynamicProp->GetRigidbody()->setIsAllowedToSleep(true);
	dynamicProp->GetRigidbody()->setUserData(dynamicProp);

	dynamicProp->SetCollider(collisionShape, collisionData->collisionTransform);
	dynamicProp->GetCollider()->getMaterial().setBounciness(0.1f);

	gameWorld->AddGameObject(dynamicProp);
	if (addAsPaintable) 
		gameWorld->AddPaintableObject(dynamicProp);

	if (networkObjectList) {
		ToonNetworkObject* netO = new ToonNetworkObject(dynamicProp, dynamicProp->GetWorldID());
		networkObjectList->push_back(netO);
	}

	return dynamicProp;
}

ToonScreen* NCL::CSC8503::ToonLevelManager::AddPropScreen(MeshGeometry* mesh, ObjectCollisionShapeData* collisionData, reactphysics3d::BodyType rigidbodyType, const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, ToonMeshMaterial* mat, Vector4 minimapColour, float mass, float addAsPaintable, float addAsFloor)
{
	ToonScreen* dynamicProp = new ToonScreen(gameWorld->GetPhysicsWorld(), gameWorld);
	dynamicProp->GetTransform().SetPosition(position).
		SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z)).
		SetScale(scale);

	//dynamicProp->SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z));

	dynamicProp->SetRenderObject(new ToonRenderObject(&dynamicProp->GetTransform(), mesh, mat, GetShader("sceneScreen")));
	dynamicProp->GetRenderObject()->SetMinimapColour(minimapColour);

	if (addAsFloor) dynamicProp->SetAsFloor();

	reactphysics3d::CollisionShape* collisionShape = nullptr;
	switch (collisionData->shapeType)
	{
		case BasicCollisionShapeType::Shape_Box:
		{
			BoxCollisionShapeData* boxData = static_cast<BoxCollisionShapeData*>(collisionData);
			collisionShape = gameWorld->GetPhysicsCommon().createBoxShape(ToonUtils::ConvertToRP3DVector3(boxData->boxHalfSize));
			break;
		}

		case BasicCollisionShapeType::Shape_Sphere:
		{
			SphereCollisionShapeData* sphereData = static_cast<SphereCollisionShapeData*>(collisionData);
			collisionShape = gameWorld->GetPhysicsCommon().createSphereShape(sphereData->radius);
			break;
		}

		case BasicCollisionShapeType::Shape_Capsule:
		{
			CapsuleCollisionShapeData* capsuleData = static_cast<CapsuleCollisionShapeData*>(collisionData);
			collisionShape = gameWorld->GetPhysicsCommon().createCapsuleShape(capsuleData->radius, capsuleData->height);
			break;
		}
	}

	dynamicProp->AddRigidbody();
	dynamicProp->GetRigidbody()->setType(rigidbodyType);
	dynamicProp->GetRigidbody()->setMass(mass);
	dynamicProp->GetRigidbody()->setIsAllowedToSleep(true);
	dynamicProp->GetRigidbody()->setUserData(dynamicProp);

	dynamicProp->SetCollider(collisionShape, collisionData->collisionTransform);
	dynamicProp->GetCollider()->getMaterial().setBounciness(0.1f);

	gameWorld->AddGameObject(dynamicProp);
	if (addAsPaintable)
		gameWorld->AddPaintableObject(dynamicProp);

	return dynamicProp;
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

	player->GetCollider()->getMaterial().setBounciness(0.1f);

	player->GetRigidbody()->setUserData(player);

	player->SetRenderObject(new ToonRenderObject(&player->GetTransform(), GetMesh("player"), GetMaterial("mat_player"), GetShader("animated"), GetMesh("arrow")));
	player->GetRenderObject()->SetMinimapColour(Vector4(team->GetTeamColour(), 1.0f));

	MeshGeometry* teamPlayerMesh = GetMesh("player_mesh_" + std::to_string(team->GetTeamID()));
	player->GetRenderObject()->SetMesh(teamPlayerMesh);	//Eg: player_mesh_1, player_mesh_2, etc

	gameWorld->AddGameObject(player);
	gameWorld->AddPaintableObject(player);

	return player;
}

PaintBallProjectile* ToonLevelManager::AddPaintBallProjectileToWorld(const reactphysics3d::Vector3& position,
	const reactphysics3d::Vector3& rotationEuler, const float& radius, const float& _impactSize, Team* team) {
	PaintBallProjectile* paintball = new PaintBallProjectile(gameWorld->GetPhysicsWorld(), gameWorld, _impactSize, team);
	paintball->AddRigidbody();
	paintball->SetPosition(position);
	paintball->SetOrientation(rotationEuler);
	paintball->GetTransform().SetScale(Vector3(radius, radius, radius));

	paintball->SetRenderObject(new ToonRenderObject(&paintball->GetTransform(), GetMesh("sphere"), GetTexture("basic"), GetShader("scene")));
	paintball->GetRenderObject()->SetColour(Vector4(team->GetTeamColour(), 1.0f));

	paintball->GetRigidbody()->setType(reactphysics3d::BodyType::DYNAMIC);
	paintball->GetRigidbody()->setMass(reactphysics3d::decimal(0.1));

	reactphysics3d::SphereShape* sphereShape = gameWorld->GetPhysicsCommon().createSphereShape(radius);
	paintball->SetCollisionShape(sphereShape);
	paintball->SetCollider(sphereShape);
	paintball->SetColliderLayer(ToonCollisionLayer::Paintball);
	int collisionMask = ToonCollisionLayer::Default | ToonCollisionLayer::Character;
	paintball->SetColliderLayerMask(ToonCollisionLayer(collisionMask));
	paintball->GetCollider()->getMaterial().setBounciness(0.1f);

	paintball->GetRigidbody()->setUserData(paintball);

	gameWorld->AddPaintball(paintball);
	return paintball;
}

HitSphere* ToonLevelManager::AddHitSphereToWorld(const reactphysics3d::Vector3& position, const float radius, Team* team) {
	HitSphere* hitSphere = new HitSphere(gameWorld->GetPhysicsWorld(), gameWorld, team, radius);
	hitSphere->AddRigidbody();
	hitSphere->SetPosition(position);
	hitSphere->GetTransform().SetScale(Vector3(radius, radius, radius));

	hitSphere->GetRigidbody()->setType(reactphysics3d::BodyType::DYNAMIC);

	reactphysics3d::SphereShape* sphereShape = gameWorld->GetPhysicsCommon().createSphereShape(radius);
	hitSphere->SetCollisionShape(sphereShape);
	hitSphere->SetCollider(sphereShape);
	hitSphere->SetColliderLayer(ToonCollisionLayer::Hitsphere);
	int collisionMask = ToonCollisionLayer::Default | ToonCollisionLayer::Character;
	hitSphere->SetColliderLayerMask(ToonCollisionLayer(collisionMask));
	hitSphere->GetCollider()->getMaterial().setBounciness(0.1f);

	//GetCollider()->setIsTrigger(true);

	hitSphere->GetRigidbody()->setUserData(hitSphere);

	gameWorld->AddHitSphere(hitSphere);
	return hitSphere;
}