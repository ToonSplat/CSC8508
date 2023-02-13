#pragma once
#include "OGLRenderer.h"
#include "OGLShader.h"
#include "OGLTexture.h"
#include "OGLMesh.h"

#include "GameWorld.h"
#include "ToonGameWorld.h"

namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;
	namespace CSC8503 {
		class RenderObject;
		class ToonRenderObject;
		class ToonFollowCamera;
		class GameTechRenderer : public OGLRenderer	{
		public:
			GameTechRenderer();		
			~GameTechRenderer();

			MeshGeometry*	LoadMesh(const string& name);
			TextureBase*	LoadTexture(const string& name);
			ShaderBase*		LoadShader(const string& vertex, const string& fragment);

			void SetWorld(ToonGameWorld* world) { gameWorld = world; }
			void ShowMinimap(bool visible = true) { minimapEnabled = visible; }
			bool IsMinimapVisible() { return minimapEnabled; }

		protected:

			void SetupStuffs();
			void GenerateShadowFBO();
			void NewRenderLines();
			void NewRenderText();

			void RenderFrame()	override;
			void RenderImGUI();

			void PresentScene();

			void DrawMinimapToScreen(int modelLocation);

			void DrawMinimap();

			void DrawMainScene();

			OGLShader*		defaultShader;

			ToonGameWorld*	gameWorld = nullptr;			

			void BuildObjectList();
			void SortObjectList();
			void RenderShadowMap();
			void RenderCamera();
			void RenderMinimap();
			void PassImpactPointDetails(const NCL::CSC8503::ToonRenderObject* const& i, int impactPointCountLocation, int& impactPointsLocation, NCL::Rendering::OGLShader* shader);

			void RenderSkybox();

			void LoadSkybox();

			void SetDebugStringBufferSizes(size_t newVertCount);
			void SetDebugLineBufferSizes(size_t newVertCount);

			vector<const ToonGameObject*> activeObjects;

			OGLShader*  debugShader;
			OGLShader*  skyboxShader;
			OGLShader*	minimapShader;
			OGLShader* textureShader;
			OGLShader* sceneShader;

			OGLMesh*	skyboxMesh;
			GLuint		skyboxTex;

			//shadow mapping things
			OGLShader*	shadowShader;
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

			bool minimapEnabled = true;

			GLuint sceneFBO;
			GLuint sceneColourTexture;
			GLuint sceneDepthTexture;
			void GenerateSceneFBO(int width, int height);

			GLuint minimapFBO;
			GLuint minimapColourTexture;
			GLuint minimapDepthTexture;
			void GenerateMinimapFBO(int width, int height);

			OGLMesh* fullScreenQuad;
			OGLMesh* minimapQuad;
			OGLMesh* minimapStencilQuad;
		};
	}
}

