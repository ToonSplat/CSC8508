#pragma once
#include <map>
#include <string>
#include "OGLShader.h"
#include "OGLTexture.h"
#include "OGLMesh.h"
#include "MeshAnimation.h"

using std::map;
using std::string;

namespace NCL {
	class ToonAssetManager {
	public:
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

	protected:
		ToonAssetManager(void);
		~ToonAssetManager(void);
		static ToonAssetManager* instance;

		Rendering::TextureBase*		AddTexture(const string& name, const string& fileName, const bool& invert = false);
		MeshGeometry*				AddMesh(const string& name, const string& fileName, const GeometryPrimitive& type = GeometryPrimitive::Triangles);
		Rendering::OGLShader*		AddShader(const string& name, const string& vertexShader, const string& fragmentShader,
			const string& geometryShader = "", const string& domainShader = "", const string& hullShader = "");
		MeshAnimation*				AddAnimation(const string& name, const string& fileName);


		map<string, Rendering::TextureBase*> textures;
		map<string, MeshGeometry*> meshes;
		map<string, Rendering::OGLShader*> shaders;
		map<string, MeshAnimation*> animations;
	};
}