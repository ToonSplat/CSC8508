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

		void LoadAssets(void);
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
		MeshGeometry*				AddMesh(const string& name, const string& fileName, const GeometryPrimitive& type = GeometryPrimitive::Triangles);
		void						AddMesh(const string& name, MeshGeometry* newMesh);
		Rendering::OGLShader*		AddShader(const string& name, const string& vertexShader, const string& fragmentShader,
			const string& geometryShader = "", const string& domainShader = "", const string& hullShader = "");
		MeshAnimation*				AddAnimation(const string& name, const string& fileName);
		ToonMeshMaterial*			AddMaterial(const string& name, const string& fileName, const unsigned int& subMeshCount);
		
		MeshGeometry* CreateCharacterTeamMesh(const std::string& fileName, const Vector4& teamColor);

		map<string, Rendering::TextureBase*> textures;
		map<string, MeshGeometry*> meshes;
		map<string, Rendering::OGLShader*> shaders;
		map<string, MeshAnimation*> animations;
		map<string, ToonMeshMaterial*> materials;
	};
}