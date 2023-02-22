#pragma once
#include "MeshMaterial.h"
#include "TextureLoader.h"
#include "OGLTexture.h"
#include "Assets.h"
#include <stb/stb_image.h>

namespace NCL
{
	class ToonMeshMaterial
	{
	public:
		ToonMeshMaterial() = delete;
		ToonMeshMaterial(const std::string& fileName, const unsigned int& subMeshCount)
		{
			material = new MeshMaterial(fileName);
			if (material != nullptr)
			{
				//material->LoadTextures();
				std::cout << "----------------------------------------------------------------------------\n";
				std::cout << "Loading Material for File: " << fileName << std::endl;
				for (unsigned int i = 0; i < subMeshCount; i++)
				{
					const MeshMaterialEntry* matEntry = material->GetMaterialForLayer(i);

					const std::string* diffuseFileName = nullptr;
					matEntry->GetEntry("Diffuse", &diffuseFileName);
					if (diffuseFileName != nullptr)
					{
						std::string filePath = Assets::TEXTUREDIR + *diffuseFileName;
						stbi_set_flip_vertically_on_load(true);
						Rendering::TextureBase* diffuseTex = TextureLoader::LoadAPITexture(filePath);
						stbi_set_flip_vertically_on_load(false);
						if (diffuseTex != nullptr)
						{
							std::cout << "Loaded Diffuse texture at path: " << filePath << ", with ID: " << dynamic_cast<Rendering::OGLTexture*>(diffuseTex)->GetObjectID() << std::endl;
							texturesDiffuse.emplace_back(diffuseTex);
						}
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
				std::cout << "----------------------------------------------------------------------------\n";
			}
		}

		std::vector<const Rendering::TextureBase*> GetDiffuseTextures() const { return std::vector<const Rendering::TextureBase*>(texturesDiffuse.begin(), texturesDiffuse.end()); }
		std::vector<const Rendering::TextureBase*> GetBumpTextures() const { return std::vector<const Rendering::TextureBase*>(texturesBump.begin(), texturesBump.end()); }

		std::vector<Rendering::TextureBase*> texturesDiffuse;
		std::vector<Rendering::TextureBase*> texturesBump;

	protected:
		MeshMaterial* material;
	};
}