#pragma once

#include "ToonAssetManager.h"

namespace NCL
{
	class ToonMeshMaterial
	{
	public:
		ToonMeshMaterial() = delete;
		ToonMeshMaterial(const std::string& fileName, const unsigned int& subMeshCount);

		std::vector<const Rendering::TextureBase*> GetDiffuseTextures() const { return std::vector<const Rendering::TextureBase*>(texturesDiffuse.begin(), texturesDiffuse.end()); }
		std::vector<const Rendering::TextureBase*> GetBumpTextures() const { return std::vector<const Rendering::TextureBase*>(texturesBump.begin(), texturesBump.end()); }

		std::vector<Vector4> GetSubMaterials() const { return std::vector<Vector4>(subMeshMaterials.begin(), subMeshMaterials.end()); }
		std::vector<int> GetMaterialIndex() const { return std::vector<int>(materialIndexes.begin(), materialIndexes.end()); }

		void AddMaterialIndex(int index) { materialIndexes.push_back(index); }
	protected:
		void LoadTextures(const std::string& entryName, const MeshMaterialEntry* materialEntry, std::vector<Rendering::TextureBase*>& texturesContainer);
		void LoadTextures(const std::string& entryName, const MeshMaterialEntry* materialEntry, std::vector<Rendering::TextureBase*>& texturesContainer, int& textureIndex);

		MeshMaterial* material;
		std::vector<Rendering::TextureBase*> texturesDiffuse;
		std::vector<Rendering::TextureBase*> texturesBump;

		std::vector<Vector4> subMeshMaterials;
		std::vector<int> materialIndexes;

		int materialIndex;
	};
}