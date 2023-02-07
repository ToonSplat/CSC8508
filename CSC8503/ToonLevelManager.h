#pragma once
#include "GameTechRenderer.h"
#include "PaintableZone.h"
#include "PaintableObject.h"

#include "Player.h"
#include "PaintBallProjectile.h"
#include <reactphysics3d/reactphysics3d.h>

namespace NCL
{
	namespace CSC8503
	{
		enum Axes
		{
			None = 0,
			X = 1,
			Y = 2,
			Z = 4
		};

		class ToonLevelManager
		{
		public:
			Player* AddPlayerToWorld(const Vector3& position);
			
			ToonLevelManager(GameTechRenderer& renderer);
			~ToonLevelManager();

			MeshGeometry* GetMesh(std::string meshName) const { 
				if (meshMap.count(meshName) == 0) {
					std::cout << "ERROR: Attempting to get Mesh that isn't loaded\n";
					return nullptr;
				}
				else return meshMap.at(meshName); 
			}
			TextureBase* GetTexture(std::string textureName) const {
				if (textureMap.count(textureName) == 0) {
					std::cout << "ERROR: Attempting to get Texture that isn't loaded\n";
					return nullptr;
				}
				else return textureMap.at(textureName);
			}
			ShaderBase* GetShader(std::string shaderName) const {
				if (shaderMap.count(shaderName) == 0) {
					std::cout << "ERROR: Attempting to get Shader that isn't loaded\n";
					return nullptr;
				}
				else return shaderMap.at(shaderName);
			}

			static ToonLevelManager* Get() { return instance; }

			void Update(float dt);

		protected:
			bool LoadAssets();
			bool LoadModel(MeshGeometry** mesh, const std::string& meshFileName);
			bool LoadTexture(TextureBase** tex, const std::string& textureFileName, const bool& invert = false);
			bool LoadShader(ShaderBase** shader, const std::string& shaderVertexShader, const std::string& shaderFragmentShader);
			bool LoadLevel();
			bool LoadPrototypeLevel();

			Axes selectedAxes = Axes::None;

			inline void SetSelectedAxes(Axes axes) {
				selectedAxes = axes;
			}

			inline bool IsXSelected() {
				return (selectedAxes & Axes::X) == Axes::X;
			}

			inline bool IsYSelected() {
				return (selectedAxes & Axes::Y) == Axes::Y;
			}

			inline bool IsZSelected() {
				return (selectedAxes & Axes::Z) == Axes::Z;
			}

			ToonGameObject* AddCubeToWorld(const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, TextureBase* cubeTex, float mass = 1.0f);
			ToonGameObject* AddSphereToWorld(const Vector3& position, const Vector3& rotationEuler, const float& radius, TextureBase* sphereTex, float mass = 1.0f);
			void AddGridWorld(Axes axes, const Vector3& gridSize, const float& gridSpacing, const Vector3& gridPosition, const Vector3& cubeScale, const float& cubeMass, TextureBase* cubeTex);


		private:
			std::map<std::string, MeshGeometry*> meshMap;
			std::map<std::string, TextureBase*> textureMap;
			std::map<std::string, ShaderBase*> shaderMap;

			GameTechRenderer& gameRenderer;			

			ToonGameObject* axisObject;

			static ToonLevelManager* instance;
		};
	}
}
