#pragma once
#include "GameTechRenderer.h"

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
			ToonLevelManager(GameTechRenderer& renderer);
			~ToonLevelManager();

		protected:
			bool LoadAssets();
			bool LoadModel(MeshGeometry** mesh, const std::string& meshFileName);
			bool LoadTexture(TextureBase** tex, const std::string& textureFileName, const bool& invert = false);
			bool LoadShader(ShaderBase** shader, const std::string& shaderVertexShader, const std::string& shaderFragmentShader);
			bool LoadLevel();

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

			GameObject* AddCubeToWorld(const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, TextureBase* cubeTex, float inverseMass = 10.0f);
			void AddGridWorld(Axes axes, const Vector3& gridSize, const float& gridSpacing, const Vector3& gridPosition, const Vector3& cubeScale, const float& cubeMass, TextureBase* cubeTex);

		private:
			MeshGeometry* cubeMesh;
			TextureBase* basicTex;
			TextureBase* basicTexPurple;
			ShaderBase* basicShader;

			GameTechRenderer& gameRenderer;

			GameObject* axisObject;
		};
	}
}