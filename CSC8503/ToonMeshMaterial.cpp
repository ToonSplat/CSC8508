#include "ToonMeshMaterial.h"


NCL::ToonMeshMaterial::ToonMeshMaterial(const std::string& fileName, const unsigned int& subMeshCount)
{	
	material = new MeshMaterial(fileName);
	if (material != nullptr)
	{
		std::cout << "----------------------------------------------------------------------------\n";
		std::cout << "Loading Material for File: " << fileName << std::endl;
		for (unsigned int i = 0; i < subMeshCount; i++)
		{
			const MeshMaterialEntry* matEntry = material->GetMaterialForLayer(i);
			//Vector2 gpuMaterial(0, 1);
			LoadTextures("Diffuse", matEntry, texturesDiffuse);
			LoadTextures("Bump", matEntry, texturesBump);
			//subMeshMaterials.push_back(gpuMaterial);
#pragma region MOVED TO LoadTextures()
			/*const std::string* diffuseFileName = nullptr;
			matEntry->GetEntry("Diffuse", &diffuseFileName);
			if (diffuseFileName != nullptr)
			{
				std::string filePath = Assets::TEXTUREDIR + *diffuseFileName;
				Rendering::TextureBase* diffuseTex = ToonAssetManager::Instance().AddTexture(*diffuseFileName, filePath, true);

				/*stbi_set_flip_vertically_on_load(true);
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
			}*/
#pragma endregion
		}
		std::cout << "----------------------------------------------------------------------------\n";
	}
	
}

void NCL::ToonMeshMaterial::LoadTextures(const std::string& entryName, const MeshMaterialEntry* materialEntry, std::vector<Rendering::TextureBase*>& texturesContainer)
{
	const std::string* textureFileName = nullptr;
	materialEntry->GetEntry(entryName, &textureFileName);
	if (textureFileName != nullptr)
	{
		std::string filePath = Assets::TEXTUREDIR + *textureFileName;
		Rendering::TextureBase* tex = ToonAssetManager::Instance().AddTexture(*textureFileName, filePath, true);

		//NCL::Rendering::OGLTexture* texture = (NCL::Rendering::OGLTexture*)tex;

		if (tex != nullptr)
		{
			std::cout << "Loaded " << entryName << " texture at path : " << filePath << ", with ID : " << dynamic_cast<Rendering::OGLTexture*>(tex)->GetObjectID() << std::endl;
			texturesContainer.emplace_back(tex);
		}
	}
}