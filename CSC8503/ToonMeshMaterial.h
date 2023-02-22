#pragma once
#include "ToonAssetManager.h"

namespace NCL
{
	class ToonMeshMaterial
	{
	public:
		ToonMeshMaterial() = delete;
		ToonMeshMaterial(const std::string& fileName, const int& subMeshCount)
		{
			material = new MeshMaterial(fileName);
			if (material != nullptr)
			{
				//material->LoadTextures();
				for (int i = 0; i < subMeshCount; ++i)
				{
					const MeshMaterialEntry* matEntry = material->GetMaterialForLayer(i);

					const std::string* diffuseFileName = nullptr;
					matEntry->GetEntry("Diffuse", &diffuseFileName);
					if (diffuseFileName != nullptr)
					{
						std::string filePath = Assets::TEXTUREDIR + *diffuseFileName;
						Rendering::TextureBase* diffuseTex = TextureLoader::LoadAPITexture(filePath);
						texturesDiffuse.emplace_back(diffuseTex);
					}

					const std::string* bumpFileName = nullptr;
					matEntry->GetEntry("Bump", &bumpFileName);
					if (bumpFileName != nullptr)
					{
						std::string bumpFilePath = Assets::TEXTUREDIR + *bumpFileName;
						Rendering::TextureBase* bumpTex = TextureLoader::LoadAPITexture(bumpFilePath);
						texturesBump.emplace_back(bumpTex);
					}
				}
			}
		}

		std::vector<const Rendering::TextureBase*> GetDiffuseTextures() const { return std::vector<const Rendering::TextureBase*>(texturesDiffuse.begin(), texturesDiffuse.end()); }
		std::vector<const Rendering::TextureBase*> GetBumpTextures() const { return std::vector<const Rendering::TextureBase*>(texturesBump.begin(), texturesBump.end()); }

	private:
		MeshMaterial* material;
		std::vector<Rendering::TextureBase*> texturesDiffuse;
		std::vector<Rendering::TextureBase*> texturesBump;
	};
}