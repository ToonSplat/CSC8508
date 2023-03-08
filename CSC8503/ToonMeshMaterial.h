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

		std::vector<const Vector2> GetSubMaterials() const { return std::vector<const Vector2>(subMeshMaterials.begin(), subMeshMaterials.end()); }
	protected:
		void LoadTextures(const std::string& entryName, const MeshMaterialEntry* materialEntry, std::vector<Rendering::TextureBase*>& texturesContainer);
		void LoadTextures(const std::string& entryName, const MeshMaterialEntry* materialEntry, std::vector<Rendering::TextureBase*>& texturesContainer, int& textureIndex);

		MeshMaterial* material;
		std::vector<Rendering::TextureBase*> texturesDiffuse;
		std::vector<Rendering::TextureBase*> texturesBump;

		std::vector<Vector2> subMeshMaterials;

		int materialIndex;
	};
}