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
		#define ATOMIC_COUNT 5
		public:
			GameTechRenderer();		
			~GameTechRenderer();

			void SetWorld(ToonGameWorld* world);
			void ShowMinimap(bool visible = true) { minimapEnabled = visible; }
			bool IsMinimapVisible() { return minimapEnabled; }
			void RetrieveAtomicValues();
		protected:

			void SetupStuffs();
			void GenerateShadowFBO();
			void NewRenderLines();
			void NewRenderText();
			void NewRenderLinesOnOrthographicView();


			void RenderFrame()	override;
			void RenderImGUI();

			void PresentScene();

			void PresentGameScene();

			void PresentMinimap(int modelLocation);

			void DrawMinimap();
			void DrawScoreBar();

			void CalculatePercentages(const int& totalPixels, const int& team1Pixels, const int& team2Pixels, const int& team3Pixels, const int& team4Pixels);

			void DrawMap();

			void DrawMainScene();

			OGLShader*		defaultShader;

			ToonGameWorld*	gameWorld = nullptr;			

			void BuildObjectList();
			void SortObjectList();
			void RenderShadowMap();

			void RenderScene(OGLShader* shader, Matrix4 viewMatrix, Matrix4 projMatrix);
			void PassImpactPointDetails(PaintableObject* const& paintedObject, OGLShader* shader);

			void RenderSkybox();

			void LoadSkybox();

			void SetDebugStringBufferSizes(size_t newVertCount);
			void SetDebugLineBufferSizes(size_t newVertCount);

			vector<ToonGameObject*> activeObjects;

			OGLShader*  debugShader;
			OGLShader*  skyboxShader;
			OGLShader*	minimapShader;
			OGLShader* scoreBarShader;
			OGLShader*  mapShader;
			OGLShader*  textureShader;
			OGLShader*  sceneShader;


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

			GLuint mapFBO;
			GLuint mapColourTexture;
			GLuint mapScoreTexture;
			GLuint mapDepthTexture;
			void GenerateMapFBO(int width, int height);

			GLuint atomicsBuffer[3];
			void GenerateAtomicBuffer();
			void ResetAtomicBuffer();
			
			GLuint teamPixelCount[ATOMIC_COUNT - 1];
			GLuint totalPixelCount;
			GLuint maxPixelCount;

			OGLMesh* fullScreenQuad;
			OGLMesh* minimapQuad;
			OGLMesh* minimapStencilQuad;

			OGLMesh* scoreQuad;

			float team1Percentage;
			float team2Percentage;
			float team3Percentage;
			float team4Percentage;

			Vector3 defaultColour = Vector3(0.5, 0.5, 0.5);

			Vector3 teamColours[ATOMIC_COUNT - 1];


			GLuint currentAtomicCPU;
			GLuint currentAtomicGPU;
			GLuint curretAtomicReset;

			
		};
	}
}

