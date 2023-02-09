#pragma once
#include "OGLRenderer.h"
#include "OGLShader.h"
#include "OGLTexture.h"
#include "OGLMesh.h"
#include "GameWorld.h"


namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;
	
	namespace CSC8503 {
		class RenderObject;
		class Light;
		class GameTechRenderer : public OGLRenderer {
		public:
			GameTechRenderer(GameWorld& world);
			~GameTechRenderer();

			MeshGeometry* LoadMesh(const string& name);
			TextureBase* LoadTexture(const string& name);
			ShaderBase* LoadShader(const string& vertex, const string& fragment);

		protected:
			void NewRenderLines();
			void NewRenderText();

			void RenderFrame()	override;

			OGLShader* defaultShader;

			GameWorld& gameWorld;

			void BuildObjectList();
			void SortObjectList();
			void RenderShadowMap();
			void RenderCamera();
			void RenderSkybox();

			void LoadSkybox();

			void SetDebugStringBufferSizes(size_t newVertCount);
			void SetDebugLineBufferSizes(size_t newVertCount);

			vector<const RenderObject*> activeObjects;
			OGLMesh* GenerateQuad;
			OGLShader* debugShader;
			OGLShader* skyboxShader;
			OGLMesh* skyboxMesh;
			GLuint		skyboxTex;
			// deffered render things
			void LoadDeferedLighting();
			void FillBuffers();
			void DrawPointLights();
			void CombineBuffers();
			OGLShader* PointLightShader;
			OGLShader* SceneShader;
			OGLShader* CombineShader;
			OGLMesh* quad;
			OGLMesh* sphere;
			Light* pointLights;
			GLuint bufferFBO;
			GLuint bufferColourTex;
			GLuint bufferNormalTex;
			GLuint bufferDepthTex;
			void GenerateScreenTexture(GLuint& into, bool depth);
			void UpdateShaderMatrices();
			OGLMesh* GenQuad();
			GLuint pointLightFBO;
			GLuint lightDiffuseTex;
			GLuint lightSpecularTex;
			void SetShaderLight(const Light& l);
			//shadow mapping things
			OGLShader* shadowShader;
			GLuint		shadowTex;
			GLuint		shadowFBO;
			Matrix4     shadowMatrix;

			Vector4		lightColour;
			float		lightRadius;
			Vector3		lightPosition;

			//Debug data storage things
			vector<Vector3> debugLineData;

			vector<Vector3> debugTextPos;
			vector<Vector4> debugTextColours;
			vector<Vector2> debugTextUVs;

			GLuint lineVAO;
			GLuint lineVertVBO;
			size_t lineCount;

			GLuint textVAO;
			GLuint textVertVBO;
			GLuint textColourVBO;
			GLuint textTexVBO;
			size_t textCount;
		};
	}
}

