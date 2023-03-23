#include "ToonAssetManager.h"
#include <stb/stb_image.h>
#include "ToonDebugManager.h"

using namespace NCL;

ToonAssetManager* ToonAssetManager::instance = NULL;

ToonAssetManager::ToonAssetManager(void) {
	
}

ToonAssetManager::~ToonAssetManager(void) {
	for (auto& [name, texture] : textures)
		delete texture;
	for (auto& [name, mesh] : meshes)
		delete mesh;
	for (auto& [name, shader] : shaders)
		delete shader;
	for (auto& [name, animation] : animations)
		delete animation;
	for (auto& [name, mat] : materials)
		delete mat;
	for (auto& [name, sound] : sounds)
		Audio::RemoveSound(sound);
}

void ToonAssetManager::LoadAssets(void) {
	ToonDebugManager::Instance().StartLoad();
	for (auto& [name, texture] : textures)
		delete texture;
	for (auto& [name, mesh] : meshes)
		delete mesh;
	for (auto& [name, shader] : shaders)
		delete shader;
	for (auto& [name, animation] : animations)
		delete animation;
	for (auto& [name, mat] : materials)
		delete mat;
	for (auto& [name, sound] : sounds)
		Audio::RemoveSound(sound);
	textures.clear();
	meshes.clear();
	shaders.clear();
	animations.clear();
	materials.clear();
	sounds.clear();
	//-----------------------------------------------------------
	//		Textures
	AddTexture("mesh", "checkerboard.png");
	AddTexture("basic", "Prefab_Grey50.png", true);
	AddTexture("basicPurple", "Prefab_Purple.png", true);
	AddTexture("crosshair", "crosshair2.png",true);
	/*AddTexture("player", "Boss_diffuse.png", true);
	AddTexture("tex_arena_wall", "RB_Level_Arena_Wall.png", true);
	AddTexture("tex_arena_wall2", "RB_Level_Arena_Wall2.png", true);
	AddTexture("tex_arena_lights", "RB_Level_Arena_Lights.png", true);*/

	//-----------------------------------------------------------
	//		Meshes
	AddMesh("cube", "cube.msh");
	AddMesh("arrow", "Minimap_Arrow.msh");
	AddMesh("player", "Character_Boss.msh");
	AddMesh("sphere", "sphere.msh");
	AddMesh("arena_main", "Level_Arena.msh");
	AddMesh("arena_lights", "Level_Arena_Lights.msh");
	AddMesh("arena_obstacles", "Level_Arena_Obstables.msh");
	AddMesh("arena_ramps", "Level_Arena_Ramps.msh");
	AddMesh("arena_decos", "Level_Arena_Decos.msh");
	AddMesh("arena_border_wall", "Level_Arena_Border.msh");

	AddMesh("player_mesh_1", CreateCharacterTeamMesh("Character_Boss.msh", Vector4(Team::T_GREEN_GOBLINS, 1.0f)));
	AddMesh("player_mesh_2", CreateCharacterTeamMesh("Character_Boss.msh", Vector4(Team::T_PURPLE_PRAWNS, 1.0f)));
	AddMesh("player_mesh_3", CreateCharacterTeamMesh("Character_Boss.msh", Vector4(Team::T_BLUE_BULLDOGS, 1.0f)));
	AddMesh("player_mesh_4", CreateCharacterTeamMesh("Character_Boss.msh", Vector4(Team::T_ORANGE_OTTERS, 1.0f)));
	//AddMesh("floorMain", "FloorsMain.msh");
	//AddMesh("platformMain", "Level_Platform.msh");
	
	//-----------------------------------------------------------
	//		Shaders
	AddShader("debug", "debug.vert", "debug.frag");
	AddShader("shadow", "shadowSkin.vert", "shadow.frag");
	AddShader("minimap", "minimap.vert", "minimap.frag");
	AddShader("texture", "Texture.vert", "Texture.frag");
	AddShader("scene", "scene.vert", "scene.frag");
	AddShader("scoreBar", "ScoreBar.vert", "ScoreBar.frag");
	AddShader("fullMap", "map.vert", "map.frag");
	AddShader("skybox", "skybox.vert", "skybox.frag");
	AddShader("animated", "sceneSkin.vert", "scene.frag");

	//-----------------------------------------------------------
	//		Animations
	AddAnimation("Player_Idle", "Boss_Gun_Idle.anm");
	AddAnimation("Player_Idle_Aim", "Boss_Gun_Idle_Aim.anm");
	AddAnimation("Player_Run", "Boss_Gun_Run.anm");
	AddAnimation("Player_Run_Aim_F", "Boss_Gun_Run_Aim_F.anm");
	AddAnimation("Player_Run_Aim_FL", "Boss_Gun_Run_Aim_FL.anm");
	AddAnimation("Player_Run_Aim_FR", "Boss_Gun_Run_Aim_FR.anm");
	AddAnimation("Player_Run_Aim_L", "Boss_Gun_Run_Aim_L.anm");
	AddAnimation("Player_Run_Aim_R", "Boss_Gun_Run_Aim_R.anm");
	AddAnimation("Player_Run_Aim_B", "Boss_Gun_Run_Aim_B.anm");
	AddAnimation("Player_Run_Aim_BL", "Boss_Gun_Run_Aim_BL.anm");
	AddAnimation("Player_Run_Aim_BR", "Boss_Gun_Run_Aim_BR.anm");

	//-----------------------------------------------------------
	//		Materials
	AddMaterial("mat_player", "Character_Boss.mat", GetMesh("player")->GetSubMeshCount());
	AddMaterial("mat_arena", "Level_Arena.mat", GetMesh("arena_main")->GetSubMeshCount());
	AddMaterial("mat_arena_obstacles", "Level_Arena_Obstables.mat", GetMesh("arena_obstacles")->GetSubMeshCount());
	AddMaterial("mat_arena_ramps", "Level_Arena_Ramps.mat", GetMesh("arena_ramps")->GetSubMeshCount());
	AddMaterial("mat_arena_lights", "Level_Arena_Lights.mat", GetMesh("arena_lights")->GetSubMeshCount());
	AddMaterial("mat_arena_decos", "Level_Arena_Decos.mat", GetMesh("arena_decos")->GetSubMeshCount());
	AddMaterial("mat_arena_border_wall", "Level_Arena_Border.mat", GetMesh("arena_border_wall")->GetSubMeshCount());

	//-----------------------------------------------------------
	//		Sounds
	AddSound("splatter", "splatter.wav");
	AddSound("gameMusic", "gameTune.wav");
	AddSound("menuMusic", "menuTune.wav");
	AddSound("splash", "splash.wav");
	AddSound("click", "click.wav");

	ToonDebugManager::Instance().EndLoad();
}

Rendering::TextureBase* ToonAssetManager::GetTexture(const string& name) {

	map<string, Rendering::TextureBase*>::iterator i = textures.find(name);

	if (i != textures.end())
		return i->second;
	return nullptr;
}

Rendering::TextureBase* ToonAssetManager::AddTexture(const string& name, const string& fileName, const bool& invert) {

	Rendering::TextureBase* texture = GetTexture(name);

	if (texture != nullptr)
		return texture;
	

	if (invert) {
		stbi_set_flip_vertically_on_load(true);
		texture = TextureLoader::LoadAPITexture(fileName);
		stbi_set_flip_vertically_on_load(false);
	}
	else
		texture = TextureLoader::LoadAPITexture(name);

	textures.emplace(name, texture);

	return texture;
}
Rendering::TextureBase* ToonAssetManager::AddTexture(const string& name, const string& fileName, int& textureIndex, const bool& invert) {

	Rendering::TextureBase* texture = GetTexture(name);

	if (texture != nullptr) {
		textureIndex = distance(textures.begin(), textures.find(name));
		return texture;
	}

	if (invert) {
		stbi_set_flip_vertically_on_load(true);
		texture = TextureLoader::LoadAPITexture(fileName);
		stbi_set_flip_vertically_on_load(false);
	}
	else
		texture = TextureLoader::LoadAPITexture(name);

	textures.emplace(name, texture);
	bindlessTextures.emplace(name, texture);
	textureIndex = distance(bindlessTextures.begin(), bindlessTextures.find(name));
	return texture;
}



MeshGeometry* ToonAssetManager::GetMesh(const string& name) {

	map<string, MeshGeometry*>::iterator i = meshes.find(name);

	if (i != meshes.end())
		return i->second;
	return nullptr;
}

MeshGeometry* ToonAssetManager::AddMesh(const string& name, const string& fileName, const GeometryPrimitive& type) {

	MeshGeometry* mesh = GetMesh(name);

	if (mesh != nullptr) return mesh;

	mesh = new Rendering::OGLMesh(fileName);
	mesh->SetPrimitiveType(type);
	mesh->UploadToGPU();

	meshes.emplace(name, mesh);

	return mesh;
}

void NCL::ToonAssetManager::AddMesh(const string& name, MeshGeometry* newMesh)
{
	if (newMesh == nullptr)
		return;

	meshes.emplace(name, newMesh);
}

Rendering::OGLShader* ToonAssetManager::GetShader(const string& name) {

	map<string, Rendering::OGLShader*>::iterator i = shaders.find(name);

	if (i != shaders.end())
		return i->second;
	return nullptr;
}

Rendering::OGLShader* ToonAssetManager::AddShader(const string& name, const string& vertexShader, const string& fragmentShader,
	const string& geometryShader, const string& domainShader, const string& hullShader) {

	Rendering::OGLShader* shader = GetShader(name);

	if (shader != nullptr) return shader;

	shader = new Rendering::OGLShader(vertexShader, fragmentShader, geometryShader, domainShader, hullShader);

	shaders.emplace(name, shader);

	return shader;
}

MeshAnimation* ToonAssetManager::GetAnimation(const string& name) {

	map<string, MeshAnimation*>::iterator i = animations.find(name);

	if (i != animations.end())
		return i->second;
	return nullptr;
}


MeshAnimation* ToonAssetManager::AddAnimation(const string& name, const string& fileName) {
	
	MeshAnimation* animation = GetAnimation(name);
	if (animation != nullptr) return animation;

	animation = new MeshAnimation(fileName);

	animations.emplace(name, animation);

	return animation;
}

ToonMeshMaterial* ToonAssetManager::GetMaterial(const string& name)
{
	map<string, ToonMeshMaterial*>::iterator i = materials.find(name);

	if (i != materials.end())
		return i->second;
	return nullptr;
}

ToonMeshMaterial* ToonAssetManager::AddMaterial(const string& name, const string& fileName, const unsigned int& subMeshCount)
{
	ToonMeshMaterial* mat = GetMaterial(name);
	if (mat != nullptr) return mat;

	mat = new ToonMeshMaterial(fileName, subMeshCount);
	materials.emplace(name, mat);

	int index = gpuMaterials.size();
	for (auto& material : mat->GetSubMaterials()) {
		gpuMaterials.push_back(material);
		index++;
		mat->AddMaterialIndex(index);
	}

	return mat;
}

CSC8503::Sound* ToonAssetManager::GetSound(const string& name)
{
	map<string, Sound*>::iterator i = sounds.find(name);

	if (i != sounds.end())
		return i->second;
	return nullptr;
}

CSC8503::Sound* ToonAssetManager::AddSound(const string& name, const string& fileName)
{
	CSC8503::Sound* sound = GetSound(name);
	if (sound != nullptr) return sound;

	sound = Audio::AddSound(fileName.c_str());
	sounds.emplace(fileName, sound); // Filename is placeholder

	return sound;
}

MeshGeometry* ToonAssetManager::CreateCharacterTeamMesh(const std::string& fileName, const Vector4& teamColor)
{
	MeshGeometry* copyPlayerMesh = new OGLMesh(fileName);
	if (copyPlayerMesh == nullptr) return nullptr;

	std::vector<Vector4> vertexColours;
	const std::vector<unsigned int> indices = copyPlayerMesh->GetIndexData();
	for (size_t i = 0; i < copyPlayerMesh->GetVertexCount(); i++)
		vertexColours.emplace_back(Debug::WHITE);

	const SubMesh* clothesSubMesh = copyPlayerMesh->GetSubMesh(4);
	if (clothesSubMesh == nullptr) return nullptr;

	int start = clothesSubMesh->start;
	int end = start + clothesSubMesh->count;

	for (int i = start; i < end; i += 3)
	{
		int A = indices[i + 0];
		int B = indices[i + 1];
		int C = indices[i + 2];

		vertexColours[A] = teamColor;
		vertexColours[B] = teamColor;
		vertexColours[C] = teamColor;
	}

	copyPlayerMesh->SetVertexColours(vertexColours);
	copyPlayerMesh->UploadToGPU();

	return copyPlayerMesh;
}
