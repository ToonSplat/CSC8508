#include "ToonAssetManager.h"
#include <stb/stb_image.h>
#include "ToonDebugManager.h"

using namespace NCL;

ToonAssetManager* ToonAssetManager::instance = NULL;

ToonAssetManager::ToonAssetManager(void) {
	file = std::ifstream(Assets::DATADIR + "ItemsToLoad.csv");
	if (!file.is_open()) {
		std::cerr << "Failed to open the file\n";
	}
	string line;
	while (getline(file, line)) {
		loadingData.assetCountTotal++;
	}
	file.clear();
	file.seekg(0, std::ios::beg);
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

void ToonAssetManager::LoadLoadingScreenAssets(void) {
	AddShader("debug", "debug.vert", "debug.frag");
	AddShader("skybox", "skybox.vert", "skybox.frag");
}

void ToonAssetManager::LoadNextAsset(void) {
	vector<string> tokens = SplitLine();
	
	if (tokens[0] == "Texture")
		AddTexture(tokens);
	else if (tokens[0] == "Mesh")
		AddMesh(tokens);
	else if (tokens[0] == "Shader")
		AddShader(tokens);
	else if (tokens[0] == "Animation")
		AddAnimation(tokens);
	else if (tokens[0] == "Material")
		AddMaterial(tokens);
	else if (tokens[0] == "Sound")
		AddSound(tokens);
	else
		throw "Error: Unknown asset type\n";
	loadingData.assetCountDone++;
}

vector<string> ToonAssetManager::SplitLine() {
	vector<string> result;
	size_t pos = 0;
	while (pos != string::npos) {
		size_t next_pos = currentLine.find(',', pos);
		if (next_pos == string::npos) {
			result.push_back(currentLine.substr(pos));
			break;
		}
		result.push_back(currentLine.substr(pos, next_pos - pos));
		pos = next_pos + 1;
	}
	return result;
}

Rendering::TextureBase* ToonAssetManager::GetTexture(const string& name) {

	map<string, Rendering::TextureBase*>::iterator i = textures.find(name);

	if (i != textures.end())
		return i->second;
	return nullptr;
}

Rendering::TextureBase* ToonAssetManager::AddTexture(vector<string> tokens) {
	string name, fileName;
	bool invert;

	name = tokens[1];
	fileName = tokens[2];
	if (tokens.size() >= 4)
		invert = (tokens[3] == "TRUE");
	else invert = false;

	return AddTexture(name, fileName, invert);
}

Rendering::TextureBase* ToonAssetManager::AddTexture(const string& name, const string& fileName, const bool& invert) {

	Rendering::TextureBase* texture = GetTexture(name);

	if (texture != nullptr) return texture;

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

MeshGeometry* ToonAssetManager::GetMesh(const string& name) {

	map<string, MeshGeometry*>::iterator i = meshes.find(name);

	if (i != meshes.end())
		return i->second;
	return nullptr;
}

MeshGeometry* ToonAssetManager::AddMesh(vector<string> tokens) {
	string name, fileName;
	if (tokens[1] != "Character") {

		name = tokens[1];
		fileName = tokens[2];
		// For now not doing type... but it can be done

		return AddMesh(name, fileName);
	}
	else {
		float r, g, b;
		name = tokens[2];
		fileName = tokens[3];
		r = stof(tokens[4]);
		g = stof(tokens[5]);
		b = stof(tokens[6]);
		return AddMesh(name, CreateCharacterTeamMesh(fileName, Vector4(r, g, b, 1.0f)));
	}
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

MeshGeometry* NCL::ToonAssetManager::AddMesh(const string& name, MeshGeometry* newMesh)
{
	if (newMesh == nullptr)
		return nullptr;

	meshes.emplace(name, newMesh);

	return newMesh;
}

Rendering::OGLShader* ToonAssetManager::GetShader(const string& name) {

	map<string, Rendering::OGLShader*>::iterator i = shaders.find(name);

	if (i != shaders.end())
		return i->second;
	return nullptr;
}

Rendering::OGLShader* ToonAssetManager::AddShader(vector<string> tokens) {
	string name, vertex, fragment, geometry, domain, hull;
	name = tokens[1];
	vertex = tokens[2];
	fragment = tokens[3];
	if (tokens.size() >= 5)
		geometry = tokens[4];
	if (tokens.size() >= 6)
		domain = tokens[5];
	if (tokens.size() >= 7)
		hull = tokens[6];
	// For now not doing type... but it can be done

	return AddShader(name, vertex, fragment, geometry, domain, hull);
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

MeshAnimation* ToonAssetManager::AddAnimation(vector<string> tokens) {
	string name, fileName;

	name = tokens[1];
	fileName = tokens[2];

	return AddAnimation(name, fileName);
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

ToonMeshMaterial* NCL::ToonAssetManager::AddMaterial(vector<string> tokens) {
	string name, fileName, mesh;

	name = tokens[1];
	fileName = tokens[2];
	mesh = tokens[3];

	return AddMaterial(name, fileName, GetMesh(mesh)->GetSubMeshCount());
}

ToonMeshMaterial* NCL::ToonAssetManager::AddMaterial(const string& name, const string& fileName, const unsigned int& subMeshCount)
{
	ToonMeshMaterial* mat = GetMaterial(name);
	if (mat != nullptr) return mat;

	mat = new ToonMeshMaterial(fileName, subMeshCount);
	materials.emplace(name, mat);

	return mat;
}

CSC8503::Sound* ToonAssetManager::GetSound(const string& name)
{
	map<string, Sound*>::iterator i = sounds.find(name);

	if (i != sounds.end())
		return i->second;
	return nullptr;
}

CSC8503::Sound* NCL::ToonAssetManager::AddSound(vector<string> tokens) {
	string name, fileName;

	name = tokens[1];
	fileName = tokens[2];

	return AddSound(name, fileName);
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

	const SubMesh* clothesSubMesh = copyPlayerMesh->GetSubMesh(4);		//4 = Clothes!
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
