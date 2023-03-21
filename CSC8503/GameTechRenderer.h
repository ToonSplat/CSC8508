#pragma once
#include "OGLRenderer.h"
#include "OGLShader.h"
#include "OGLTexture.h"
#include "OGLMesh.h"
#include "ToonGameWorld.h"

namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;
	namespace CSC8503 {
		class RenderObject;
		class ToonRenderObject;
		class ToonFollowCamera;
		struct LightStruct {
			Vector4 lightColour;
			Vector3 lightPosition;
			float lightRadius;
		};
		struct ShaderLights {
			LightStruct data[1];
		};

		
		
		class GameTechRenderer : public OGLRenderer	{
		#define TEAM_COUNT 4
		struct TeamColourStruct {
			Vector4 teams[4];
		} teamColoursShader;
		public:
			GameTechRenderer();		
			~GameTechRenderer();

			void SetWorld(ToonGameWorld* world);
			void ShowMinimap(bool visible = true) { minimapEnabled = visible; }
			bool IsMinimapVisible() { return minimapEnabled; }
			void RetrieveAtomicValues();
			void SetShadowSize(int size) { shadowSize = size; }
			void GenerateShadowFBO();
			std::map<int, float> GetTeamScores();

			void ResetAtomicBuffer();

			bool IsMapInitialised() { return mapInitialised; }

			bool mapNeedsDrawing = false;
		protected:

			void SetupLoadingScreen();
			void SetupMain();
			void NewRenderLines();
			void NewRenderText();
			void NewRenderLinesOnOrthographicView();

			void RenderFrameLoading();
			void CreateLightUBO();
			void CreateTeamColourUBO();


			void RenderFrame()	override;
			void UpdateLighting();
			void Render2Player();
			void Render1Player();
			void Render3or4Player();
			
			void Present1Player();
			void Present2Player();
			void Present3Player();
			void Present4Player();
			
			void RenderImGUI();

			void PresentScene();

			void PresentGameScene();

			void PresentMinimap();

			void DrawMinimap();
			void DrawScoreBar();

			void CalculatePercentages(const int& team1Pixels, const int& team2Pixels, const int& team3Pixels, const int& team4Pixels);
			int GetWinningTeam(float& percentage);

			void DrawMap();
			void UpdateMap();

			void DrawMainScene();

			void RenderRectical();

			OGLShader*		defaultShader;

			ToonGameWorld*	gameWorld = nullptr;			

			void BuildObjectList();
			void SortObjectList();
			void RenderShadowMap();

			void RenderMaps(OGLShader* shader, Matrix4 viewMatrix, Matrix4 projMatrix);
			void RenderScene();
			void PassImpactPointDetails(ToonGameObject* const& paintedObject, OGLShader* shader);

			void RenderSkybox(bool enableTests = true);

			void LoadSkybox(string fileName = "");

			void SetDebugStringBufferSizes(size_t newVertCount);
			void SetDebugLineBufferSizes(size_t newVertCount);

			void DrawLoader();

			vector<ToonGameObject*> activeObjects;

			OGLShader*  debugShader;
			OGLShader*  skyboxShader;
			OGLShader*	minimapShader;
			OGLShader*	scoreBarShader;
			OGLShader*  mapShader;
			OGLShader*  textureShader;
			OGLShader*  sceneShader;
			OGLShader*  mapInitShader;
			OGLShader* mapUpdateShader;


			OGLMesh*	skyboxMesh;
			GLuint		skyboxTex;

			//shadow mapping things
			OGLShader*	shadowShader;
			GLuint shadowFBO;
			GLuint shadowTex;
			Matrix4 shadowMatrix;
			int shadowSize;

			ShaderLights shaderLight;

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
			GLuint mapPositionTexture;
			GLuint mapDepthTexture;
			void GenerateMapFBO(int width, int height);

			GLuint splitFBO[2];
			GLuint splitColourTexture[2];
			GLuint splitDepthTexture[2];
			void GenerateSplitFBO(int width, int height);

			GLuint quadFBO[4];
			GLuint quadColourTexture[4];
			GLuint quadDepthTexture[4];
			void GenerateQuadFBO(int width, int height);

			GLuint* currentFBO;

			GLuint atomicsBuffer;
			void GenerateAtomicBuffer();
			
			
			GLuint teamPixelCount[TEAM_COUNT];
			GLuint totalPixelCount;
			GLuint maxPixelCount;

			OGLMesh* fullScreenQuad;
			OGLMesh* squareQuad;
			OGLMesh* minimapStencilQuad;

			OGLMesh* scoreQuad;

			float team1Percentage;
			float team2Percentage;
			float team3Percentage;
			float team4Percentage;

			Vector3 defaultColour = Vector3(1, 0, 0);

			Vector3 teamColours[TEAM_COUNT];


			Camera* currentRenderCamera;
			float screenAspect;

			unsigned int lightUBO;
			unsigned int teamUBO;

			bool mapInitialised = false;
			bool updateScorebar;
			

			
		};
	}
}

