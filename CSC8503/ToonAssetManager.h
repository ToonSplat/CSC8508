#pragma once
#include <map>
#include <unordered_set>
#include <string>
#include "OGLShader.h"
#include "OGLTexture.h"
#include "OGLMesh.h"
#include "MeshAnimation.h"
#include "MeshMaterial.h"
#include "TextureLoader.h"
#include "OGLTexture.h"
#include "Assets.h"
#include "ToonMeshMaterial.h"
#include <iostream>
#include <fstream>

using std::map;
using std::string;

namespace NCL {
	class ToonAssetManager {
	public:
		friend class ToonMeshMaterial;

		static void Create() {
			if (instance == NULL)
				instance = new ToonAssetManager();
		}

		static void Destroy() { delete instance; }

		static ToonAssetManager& Instance() {
			if (instance == NULL)
				Create();
			return *instance;
		}

		void LoadLoadingScreenAssets(void);
		bool AreAssetsRemaining(void) {
			if (std::getline(file, currentLine))
				return true;
			else return false;
		}
		void LoadNextAsset();
		Rendering::TextureBase* GetTexture(const string& name);
		MeshGeometry* GetMesh(const string& name);
		Rendering::OGLShader* GetShader(const string& name);
		MeshAnimation* GetAnimation(const string& name);
		ToonMeshMaterial* GetMaterial(const string& name);

	protected:
		ToonAssetManager(void);
		~ToonAssetManager(void);
		static ToonAssetManager* instance;

		Rendering::TextureBase*		AddTexture(const string& name, const string& fileName, const bool& invert = false);
		Rendering::TextureBase*		AddTexture(vector<string> tokens);

		MeshGeometry*				AddMesh(const string& name, const string& fileName, const GeometryPrimitive& type = GeometryPrimitive::Triangles);
		MeshGeometry*				AddMesh(vector<string> tokens);
		MeshGeometry*				AddMesh(const string& name, MeshGeometry* newMesh);

		Rendering::OGLShader*		AddShader(const string& name, const string& vertexShader, const string& fragmentShader,
			const string& geometryShader = "", const string& domainShader = "", const string& hullShader = "");
		Rendering::OGLShader*		AddShader(vector<string> tokens);

		MeshAnimation*				AddAnimation(const string& name, const string& fileName);
		MeshAnimation*				AddAnimation(vector<string> tokens);

		ToonMeshMaterial*			AddMaterial(const string& name, const string& fileName, const unsigned int& subMeshCount);
		ToonMeshMaterial*			AddMaterial(vector<string> tokens);
		
		MeshGeometry* CreateCharacterTeamMesh(const std::string& fileName, const Vector4& teamColor);
		vector<string> SplitLine();

		std::ifstream file;
		string currentLine;

		map<string, Rendering::TextureBase*> textures;
		map<string, MeshGeometry*> meshes;
		map<string, Rendering::OGLShader*> shaders;
		map<string, MeshAnimation*> animations;
		map<string, ToonMeshMaterial*> materials;
	};
}