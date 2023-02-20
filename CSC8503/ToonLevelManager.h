#pragma once
#include "GameTechRenderer.h"
#include "PaintableObject.h"
#include "ToonGameWorld.h"
#include "Player.h"
#include "PaintBallProjectile.h"
#include "HitSphere.h"
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
			ToonLevelManager(GameTechRenderer* renderer, ToonGameWorld* gameWorld);
			~ToonLevelManager();

			Player* AddPlayerToWorld(const Vector3& position, Team* team);
			PaintBallProjectile* AddPaintBallProjectileToWorld(const reactphysics3d::Vector3& position,
				const reactphysics3d::Vector3& rotationEuler, const float& radius, const float& _impactSize, Team* team);
			HitSphere* AddHitSphereToWorld(const reactphysics3d::Vector3& position, const float radius, Team* team);

			Player* GetPlayer() { return player; }

			void ResetLevel(std::vector<ToonNetworkObject*>* networkObjectList = nullptr);

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

			void Update(float dt);

		protected:
			bool LoadAssets();
			bool LoadModel(MeshGeometry** mesh, const std::string& meshFileName);
			bool LoadTexture(TextureBase** tex, const std::string& textureFileName, const bool& invert = false);
			bool LoadShader(ShaderBase** shader, const std::string& shaderVertexShader, const std::string& shaderFragmentShader);
			bool LoadPrototypeLevel(std::vector<ToonNetworkObject*>* networkObjectList = nullptr);

			reactphysics3d::ConcaveMeshShape* CreateConcaveMeshShape(MeshGeometry* mesh, const Vector3& scaling);

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

			PaintableObject* AddCubeToWorld(const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, TextureBase* cubeTex, Vector4 minimapColour, float mass = 1.0f, bool isFloor = false);
			PaintableObject* AddSphereToWorld(const Vector3& position, const Vector3& rotationEuler, const float& radius, TextureBase* sphereTex, Vector4 minimapColour, float mass = 1.0f);
			void AddGridWorld(Axes axes, const Vector3& gridSize, const float& gridSpacing, const Vector3& gridPosition,
							const Vector3& cubeScale, const float& cubeMass, TextureBase* cubeTex, 
							Vector4 minimapColour = Vector4(0,0,0,1), bool isFloor = false);
			PaintableObject* AddConcaveObjectToWorld(MeshGeometry* mesh, const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, TextureBase* cubeTex, Vector4 minimapColour, float mass = 1.0f);

		private:
			std::map<std::string, MeshGeometry*> meshMap;
			std::map<std::string, TextureBase*> textureMap;
			std::map<std::string, ShaderBase*> shaderMap;

			GameTechRenderer* gameRenderer;		
			ToonGameWorld* gameWorld;

			PaintableObject* axisObject;
			Player* player = nullptr;

			static ToonLevelManager* instance;
		};
	}
}
