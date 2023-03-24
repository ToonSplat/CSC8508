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
		struct CrosshairStruct
		{
			Vector3 pos;
			float rot;
			Vector3 scale;
		};
		struct ShaderLights {
			LightStruct data[1];
		};

		
		
		class GameTechRenderer : public OGLRenderer	{

		#define TEAM_COUNT 4
		struct TeamColourStruct {
			Vector4 teams[4];
		} teamColoursShader;

		#define ATOMIC_COUNT 5
		
		struct textureStruct {
			GLuint64 values[30];
		}textures;

		struct materialStruct {
			Vector4 values[100];
		} materials;

		public:
			bool m_EnableDynamicCrosshair = true;

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
			void ToggleDebug() { displayDebug = !displayDebug; }
			void OnWindowResize(int w, int h)	override;
		protected:

			void SetupLoadingScreen();
			void SetupMain();
			void GenerateQuads();
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

			void DrawPlayerIcon();

			void DrawScoreBar();

			void CalculatePercentages(const int& team1Pixels, const int& team2Pixels, const int& team3Pixels, const int& team4Pixels);
			int GetWinningTeam(float& percentage);

			void DrawMap();
			void UpdateMap();

			void DrawMainScene(int id = 1);

			void RenderRectical(int id = -1);
			CrosshairStruct crosshairs[4];
			float crosshairSpreadFactor;

			void RenderWeapon(int id);

			void RenderTeamBeacons(int id = -1);

			OGLShader*		defaultShader;

			ToonGameWorld*	gameWorld = nullptr;

			void BuildObjectList(int index = 0);
			void SortObjectList();
			void RenderShadowMap();

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
			OGLShader*	scoreBarShader;
			OGLShader*  textureShader;
			OGLShader*  sceneShader;
			OGLShader*  mapInitShader;
			OGLShader* mapUpdateShader;
			OGLShader*	sceneScreenShader;
			OGLShader* playerShader;

			OGLMesh*	skyboxMesh;
			GLuint		skyboxTex;

			//shadow mapping things
			OGLShader*	shadowShader;
			GLuint shadowFBO;
			GLuint shadowTex;
			Matrix4 shadowMatrix;
			int shadowSize;

			LightStruct sceneLight;
			void UpdateLightColour();

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
			GLuint maxPixelCount;

			OGLMesh* fullScreenQuad;
			OGLMesh* squareQuad;
			OGLMesh* minimapStencilQuad;

			OGLMesh* scoreQuad;

			OGLMesh* arrowMesh;

			float team1Percentage;
			float team2Percentage;
			float team3Percentage;
			float team4Percentage;

			Vector3 defaultColour = Vector3(0.5, 0.5,0.5);

			Vector3 teamColours[TEAM_COUNT];


			Camera* currentRenderCamera;
			float screenAspect;

			unsigned int lightUBO;
			unsigned int teamUBO;

			bool mapInitialised = false;
			bool updateScorebar;
			

			unsigned int textureUBO;
			void CreateTextureUBO();

			unsigned int materialUBO;
			void CreateMaterialUBO();

			bool displayDebug = false;
		};
	}
}

