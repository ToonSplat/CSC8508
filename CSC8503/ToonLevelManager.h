#pragma once
#include "GameTechRenderer.h"

namespace NCL
{
	namespace CSC8503
	{
		class ToonLevelManager
		{
		public:
			ToonLevelManager(GameTechRenderer& renderer);
			~ToonLevelManager();

		protected:
			bool LoadAssets();
			bool LoadModel(MeshGeometry* mesh, const std::string& meshFileName);
			bool LoadTexture(TextureBase* tex, const std::string& textureFileName, const bool& invert = false);
			bool LoadShader(ShaderBase* shader, const std::string& shaderVertexShader, const std::string& shaderFragmentShader);
			bool LoadLevel();

			GameObject* AddCubeToWorld(const Vector3& position, const Vector3& rotationEuler, const Vector3& scale, TextureBase* cubeTex, float inverseMass = 10.0f);
			void InitCubeGridWorld(const int& numRows, const int& numCols, const float& rowSpacing, const float& colSpacing, const Vector3& cubeScale, const float& cubeMass);

		private:
			MeshGeometry* cubeMesh;
			TextureBase* basicTex;
			ShaderBase* basicShader;

			GameTechRenderer& gameRenderer;
		};
	}
}
