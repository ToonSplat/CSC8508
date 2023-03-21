#include "GameTechRenderer.h"
#include "ToonGameObject.h"
#include "ToonRenderObject.h"
#include "ToonFollowCamera.h"
#include "ToonLevelManager.h"
#include "ToonUtils.h"
#include "TextureLoader.h"
#include "ImpactPoint.h"
#include "PaintableObject.h"
#include "ToonUtils.h"
#include <iostream>
#include <algorithm>
#include <reactphysics3d/reactphysics3d.h>
#include "ToonAssetManager.h"
#include "ToonDebugManager.h"
#include "Player.h"
#include "HitSphere.h"

#include "../ThirdParty/imgui/imgui.h"
#include "../ThirdParty/imgui/imgui_impl_opengl3.h"
#include "../ThirdParty/imgui/imgui_impl_win32.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

Matrix4 biasMatrix = Matrix4::Translation(Vector3(0.5f, 0.5f, 0.5f)) * Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f));

GameTechRenderer::GameTechRenderer() : OGLRenderer(*Window::GetWindow())
{
	ToonDebugManager::Instance().StartLoad();

	SetupLoadingScreen();

	while (ToonAssetManager::Instance().AreAssetsRemaining()) {
		RenderFrameLoading();
		ToonAssetManager::Instance().LoadNextAsset();
	}

	ToonDebugManager::Instance().EndLoad();

	SetupMain();
}

GameTechRenderer::~GameTechRenderer()	{
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
}

void GameTechRenderer::SetupLoadingScreen() {
	// Get the bare minimum assets needed for loadingscreen
	ToonAssetManager::Instance().LoadLoadingScreenAssets();

	// Text and Drawing
	debugShader = ToonAssetManager::Instance().GetShader("debug");

	glGenVertexArrays(1, &lineVAO);
	glGenVertexArrays(1, &textVAO);

	glGenBuffers(1, &lineVertVBO);
	glGenBuffers(1, &textVertVBO);
	glGenBuffers(1, &textColourVBO);
	glGenBuffers(1, &textTexVBO);

	SetDebugStringBufferSizes(10000);
	SetDebugLineBufferSizes(1000);

	// Background is done as a skybox
	skyboxShader = ToonAssetManager::Instance().GetShader("skybox");

	skyboxMesh = new OGLMesh();
	skyboxMesh->SetVertexPositions({ Vector3(-1, 1,-1), Vector3(-1,-1,-1) , Vector3(1,-1,-1) , Vector3(1,1,-1) });
	skyboxMesh->SetVertexIndices({ 0,1,2,2,3,0 });
	skyboxMesh->UploadToGPU();
	LoadSkybox("ToonSplat_Background.png");
}

void NCL::CSC8503::GameTechRenderer::SetupMain()
{
	glEnable(GL_DEPTH_TEST);
	shadowShader = ToonAssetManager::Instance().GetShader("shadow");
	minimapShader = ToonAssetManager::Instance().GetShader("minimap");
	textureShader = ToonAssetManager::Instance().GetShader("texture");
	sceneShader = ToonAssetManager::Instance().GetShader("scene");
	scoreBarShader = ToonAssetManager::Instance().GetShader("scoreBar");
	mapShader = ToonAssetManager::Instance().GetShader("fullMap");
	mapInitShader = ToonAssetManager::Instance().GetShader("map_initial");
	mapUpdateShader = ToonAssetManager::Instance().GetShader("map_update");

	shadowSize = 2048;
	GenerateShadowFBO();
	GenerateSceneFBO(windowWidth, windowHeight);
	GenerateSplitFBO(windowWidth / 2, windowHeight);
	GenerateQuadFBO(windowWidth / 2, windowHeight / 2);
	GenerateMinimapFBO(windowWidth, windowHeight);
	GenerateMapFBO(windowWidth, windowHeight);
	GenerateAtomicBuffer();

	screenAspect = (float)windowWidth / (float)windowHeight;

	glClearColor(1, 1, 1, 1);

	shaderLight = ShaderLights();
	shaderLight.data[0] = LightStruct(Vector4(0.8f, 0.8f, 0.5f, 1.0f), Vector3(0.0f, 500.0f, 0.0f), 500.0f); //Vector3(-300.0f, 500.0f, -300.0f)

	fullScreenQuad = new OGLMesh();
	fullScreenQuad->SetVertexPositions({ Vector3(-1, 1,1), Vector3(-1,-1,1) , Vector3(1,-1,1) , Vector3(1,1,1) });
	fullScreenQuad->SetVertexTextureCoords({ Vector2(0.0f,1.0f), Vector2(0.0f,0.0f), Vector2(1.0f,0.0f), Vector2(1.0f,1.0f) });
	fullScreenQuad->SetVertexIndices({ 0,1,2,2,3,0 });
	fullScreenQuad->UploadToGPU();
	
	squareQuad = new OGLMesh();
	squareQuad->SetVertexPositions({Vector3(-0.5f, 0.8f, -1.0f), Vector3(-0.5f, -0.8f, -1.0f), Vector3(0.5f, -0.8f, -1.0f), Vector3(0.5f, 0.8f, -1.0f)});
	squareQuad->SetVertexTextureCoords({ Vector2(0.0f,1.0f), Vector2(0.0f,0.0f), Vector2(1.0f,0.0f), Vector2(1.0f,1.0f) });
	squareQuad->SetVertexIndices({ 0,1,2,2,3,0 });
	squareQuad->UploadToGPU();

	minimapStencilQuad = new OGLMesh();
	minimapStencilQuad->SetVertexPositions({ Vector3(-0.5f, 0.8f, -1.0f), Vector3(-0.5f, -0.8f, -1.0f) , Vector3(0.5f, -0.8f, -1.0f) , Vector3(0.5f, 0.8f, -1.0f) });
	minimapStencilQuad->SetVertexIndices({ 0,1,2,2,3,0 });
	minimapStencilQuad->UploadToGPU();

	scoreQuad = new OGLMesh();
	scoreQuad->SetVertexPositions({ Vector3(-1, 1, 1), Vector3(-1, -1, 1), Vector3(1, -1, 1), Vector3(1, 1, 1) });
	scoreQuad->SetVertexTextureCoords({ Vector2(0.0f,1.0f), Vector2(0.0f,0.0f), Vector2(1.0f,0.0f), Vector2(1.0f,1.0f) });
	scoreQuad->SetVertexIndices({ 0,1,2,2,3,0 });
	scoreQuad->UploadToGPU();
	
	LoadSkybox();
	CreateLightUBO();

	team1Percentage = 0;
	team2Percentage = 0;
	team3Percentage = 0;
	team4Percentage = 0;
}

void GameTechRenderer::RenderFrame() {
	ToonDebugManager::Instance().StartRendering();
	if (!gameWorld) return; // Safety Check

	UpdateLighting();
	
	switch (gameWorld->GetMainCameraCount()) {
	case 1:
		Render1Player();
		break;
	case 2:
		Render2Player();
		break;
	default:
		Render3or4Player();
		break;
	}

	if (mapNeedsDrawing) DrawMap();
	UpdateMap();
	PresentScene();

	RenderImGUI();
	ToonDebugManager::Instance().EndRendering();
}

void NCL::CSC8503::GameTechRenderer::UpdateLighting()
{
	float percentageScale = 0.0f;
	int winning = GetWinningTeam(percentageScale);

	shaderLight.data[0].lightColour = teamColours[winning - 1] * (1 - percentageScale);

	if (percentageScale > 0.99) {
		shaderLight.data[0].lightColour = defaultColour;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, lightMatrix);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightStruct), &shaderLight, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void NCL::CSC8503::GameTechRenderer::DrawMainScene(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(1, 1, 1, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	BuildObjectList();
	RenderShadowMap();
	RenderSkybox();
	RenderScene();
	RenderRectical();
}

void GameTechRenderer::RenderRectical()
{
	if (!gameWorld->HasGameStarted()) return;
	BindShader(textureShader);

	BindTextureToShader((OGLTexture*)ToonAssetManager::Instance().GetTexture("crosshair"), "diffuseTex", 0);
	Matrix4 minimapModelMatrix = Matrix4::Translation(Vector3(-0.05f, 0.0f, 0.0f)) * Matrix4::Scale(Vector3(0.1f, 0.07f, 1.0f));

	int modelLocation = glGetUniformLocation(textureShader->GetProgramID(), "modelMatrix");
	glUniformMatrix4fv(modelLocation, 1, false, (float*)&minimapModelMatrix);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 2, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	BindMesh(squareQuad);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GameTechRenderer::RenderScene() {
	Matrix4 viewMatrix = currentRenderCamera->BuildViewMatrix();
	Matrix4 projMatrix = currentRenderCamera->BuildProjectionMatrix(screenAspect);

	OGLShader* activeShader = nullptr;
	int projLocation = 0;
	int viewLocation = 0;
	int modelLocation = 0;
	int colourLocation = 0;
	int hasVColLocation = 0;
	int hasTexLocation = 0;
	int shadowLocation = 0;
	int objectPosLocation = 0;

	int lightPosLocation = 0;
	int lightColourLocation = 0;
	int lightRadiusLocation = 0;

	int cameraLocation = 0;

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	for (const auto& i : activeObjects) {
		if ((*i).GetRenderObject() == nullptr) continue;
		ToonGameObject* linkedObject = (*i).GetRenderObject()->GetGameObject();
		if (dynamic_cast<HitSphere*>(linkedObject))
			return;

		OGLShader* shader = (OGLShader*)(*i).GetRenderObject()->GetShader();
		BindShader(shader);

		if ((*i).GetRenderObject()->GetDefaultTexture() != nullptr) BindTextureToShader((OGLTexture*)(*i).GetRenderObject()->GetDefaultTexture(), "mainTex", 0);

		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, mapColourTexture);
		glUniform1i(glGetUniformLocation(shader->GetProgramID(), "mapTex"), 7);

		if (activeShader != shader) {
			projLocation = glGetUniformLocation(shader->GetProgramID(), "projMatrix");
			viewLocation = glGetUniformLocation(shader->GetProgramID(), "viewMatrix");
			modelLocation = glGetUniformLocation(shader->GetProgramID(), "modelMatrix");
			shadowLocation = glGetUniformLocation(shader->GetProgramID(), "shadowMatrix");
			colourLocation = glGetUniformLocation(shader->GetProgramID(), "objectColour");
			hasVColLocation = glGetUniformLocation(shader->GetProgramID(), "hasVertexColours");
			hasTexLocation = glGetUniformLocation(shader->GetProgramID(), "hasTexture");
			objectPosLocation = glGetUniformLocation(shader->GetProgramID(), "objectPosition");

			lightPosLocation = glGetUniformLocation(shader->GetProgramID(), "lightPos");
			lightColourLocation = glGetUniformLocation(shader->GetProgramID(), "lightColour");
			lightRadiusLocation = glGetUniformLocation(shader->GetProgramID(), "lightRadius");

			cameraLocation = glGetUniformLocation(shader->GetProgramID(), "cameraPos");

			//Vector3 camPos = gameWorld->GetMainCamera()->GetPosition();
			//glUniform3fv(cameraLocation, 1, camPos.array);

			glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
			glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

			int shadowTexLocation = glGetUniformLocation(shader->GetProgramID(), "shadowTex");
			glUniform1i(shadowTexLocation, 1);
			activeShader = shader;
		}

		Matrix4 modelMatrix = (*i).GetModelMatrix();
		glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

		Matrix4 fullShadowMat = shadowMatrix * modelMatrix;
		glUniformMatrix4fv(shadowLocation, 1, false, (float*)&fullShadowMat);

		Vector4 colour = (*i).GetRenderObject()->GetColour();
		glUniform4fv(colourLocation, 1, colour.array);

		glUniform1i(hasVColLocation, !(*i).GetRenderObject()->GetMesh()->GetColourData().empty());
		int hasTexFlag = ((OGLTexture*)(*i).GetRenderObject()->GetDefaultTexture() || (*i).GetRenderObject()->GetMaterial() != nullptr) ? 1 : 0;
		glUniform1i(hasTexLocation, hasTexFlag);

		Vector3 objPos = ToonUtils::ConvertToNCLVector3((i)->GetRigidbody()->getTransform().getPosition());
		glUniform3fv(objectPosLocation, 1, objPos.array);

		if (dynamic_cast<PaintableObject*>(linkedObject) || dynamic_cast<Player*>(linkedObject)) {
			PassImpactPointDetails(linkedObject, shader);
		}
		else {
			int impactPointCountLocation = glGetUniformLocation(shader->GetProgramID(), "impactPointCount");
			glUniform1i(impactPointCountLocation, 0);
		}

		(*i).Draw(*this);
	}
}

void NCL::CSC8503::GameTechRenderer::Render2Player()
{
	
	screenAspect = ((float)windowWidth / 2) / (float)windowHeight;
	for (int i = 0; i < gameWorld->GetMainCameraCount(); i++)
	{
		currentFBO = &splitFBO[i];
		glBindFramebuffer(GL_FRAMEBUFFER, *currentFBO);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, windowWidth / 2, windowHeight);
		currentRenderCamera = gameWorld->GetMainCamera(i + 1);
		DrawMainScene();
		glViewport(0, 0, windowWidth, windowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
void NCL::CSC8503::GameTechRenderer::Render3or4Player()
{
	float width = windowWidth / 2;
	float height = windowHeight / 2;
	screenAspect = (width) / (height);
	

	for (int i = 0; i < gameWorld->GetMainCameraCount(); i++)
	{
		currentFBO = &quadFBO[i];
		glBindFramebuffer(GL_FRAMEBUFFER, *currentFBO);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, width, height);
		currentRenderCamera = gameWorld->GetMainCamera(i + 1);
		DrawMainScene();
		glViewport(0, 0, windowWidth, windowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
}

void NCL::CSC8503::GameTechRenderer::Render1Player()
{
	screenAspect = (float)windowWidth / (float)windowHeight;
	currentFBO = &sceneFBO;
	glBindFramebuffer(GL_FRAMEBUFFER, *currentFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	currentRenderCamera = gameWorld->GetMainCamera(1);
	DrawMainScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//DrawMinimap();
}

void GameTechRenderer::RenderMaps(OGLShader* shader, Matrix4 viewMatrix, Matrix4 projMatrix){
	BindShader(shader);
	for (const auto& i : activeObjects) {
		if ((*i).GetRenderObject() == nullptr) continue;
	
		int projLocation = glGetUniformLocation(shader->GetProgramID(), "projMatrix");
		int viewLocation = glGetUniformLocation(shader->GetProgramID(), "viewMatrix");
		int modelLocation = glGetUniformLocation(shader->GetProgramID(), "modelMatrix");
		int rotationLocation = glGetUniformLocation(shader->GetProgramID(), "rotationMatrix");
		int colourLocation = glGetUniformLocation(shader->GetProgramID(), "objectColour");
		int minimapColourLocation = glGetUniformLocation(shader->GetProgramID(), "objectMinimapColour");
		int hasVColLocation = glGetUniformLocation(shader->GetProgramID(), "hasVertexColours");
		int hasTexLocation = glGetUniformLocation(shader->GetProgramID(), "hasTexture");
		int objectPosLocation = glGetUniformLocation(shader->GetProgramID(), "objectPosition");

		ToonGameObject* linkedObject = (*i).GetRenderObject()->GetGameObject();
		if (dynamic_cast<PaintableObject*>(linkedObject)) {

			PaintableObject* paintedObject = (PaintableObject*)linkedObject;
			int isFloorLocation = glGetUniformLocation(shader->GetProgramID(), "isFloor");
			glUniform1i(isFloorLocation, paintedObject->IsObjectTheFloor() ? 1 : 0);
			PassImpactPointDetails(paintedObject, shader);
		}
		if (shader == mapShader) {
			// MAKE COLOUR WORK
			int atomicLocation = glGetUniformLocation(shader->GetProgramID(), "currentAtomicTarget");
			glUniform1i(atomicLocation, currentAtomicGPU);

			glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "team1Colour"), 1, teamColours[0].array);
			glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "team2Colour"), 1, teamColours[1].array);
			glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "team3Colour"), 1, teamColours[2].array);
			glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "team4Colour"), 1, teamColours[3].array);
		}

		glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
		glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

		Vector3 objPos = ToonUtils::ConvertToNCLVector3((i)->GetRigidbody()->getTransform().getPosition());
		glUniform3fv(objectPosLocation, 1, objPos.array);

		Matrix4 modelMatrix = (*i).GetModelMatrix();
		glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

		Vector4 colour = i->GetRenderObject()->GetColour();
		glUniform4fv(colourLocation, 1, colour.array);

		if (shader == minimapShader)
			glUniform4fv(minimapColourLocation, 1, i->GetRenderObject()->GetMinimapColour().array);

		glUniform1i(hasVColLocation, !(*i).GetRenderObject()->GetMesh()->GetColourData().empty());

		int hasTexFlag = ((OGLTexture*)(*i).GetRenderObject()->GetDefaultTexture() || (*i).GetRenderObject()->GetMaterial() != nullptr) ? 1 : 0;
		glUniform1i(hasTexLocation, hasTexFlag);

		(*i).Draw(*this, shader == minimapShader && (*i).GetRenderObject()->GetMinimapMesh() != nullptr);
	}
}

void GameTechRenderer::PresentScene(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glDisable(GL_CULL_FACE); //Todo - text indices are going the wrong way...
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	BindShader(textureShader);
	Matrix4 identityMatrix = Matrix4();

	int projLocation = glGetUniformLocation(textureShader->GetProgramID(), "projMatrix");
	int viewLocation = glGetUniformLocation(textureShader->GetProgramID(), "viewMatrix");
	int modelLocation = glGetUniformLocation(textureShader->GetProgramID(), "modelMatrix");

	glUniformMatrix4fv(modelLocation, 1, false, (float*)&identityMatrix);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&identityMatrix);
	glUniformMatrix4fv(projLocation, 1, false, (float*)&identityMatrix);

	switch (gameWorld->GetMainCameraCount()) {
	case 2:
		Present2Player();
		break;
	case 3:
		Present3Player();
		break;
	case 4:
		Present4Player();
		break;
	default:
		Present1Player();
		break;
	}


	if (gameWorld->GetMapCamera()) {
		DrawScoreBar();
	}

	
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	NewRenderLines();
	NewRenderLinesOnOrthographicView();
	NewRenderText();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
}

void NCL::CSC8503::GameTechRenderer::PresentGameScene(){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sceneColourTexture);
	glUniform1i(glGetUniformLocation(textureShader->GetProgramID(), "diffuseTex"), 0);
	BindMesh(fullScreenQuad);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void NCL::CSC8503::GameTechRenderer::PresentMinimap(){
	if (!gameWorld->GetMinimapCamera() || !mapInitialised) return;
	int modelLocation = glGetUniformLocation(textureShader->GetProgramID(), "modelMatrix");
	//glEnable(GL_STENCIL_TEST);

	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	//glStencilFunc(GL_ALWAYS, 2, ~0);
	//glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	Matrix4 minimapModelMatrix = Matrix4::Translation(Vector3(-0.8f, -0.7f, 0.0f)) * Matrix4::Scale(Vector3(0.3f, 0.3f, 1.0f));
	glUniformMatrix4fv(modelLocation, 1, false, (float*)&minimapModelMatrix);


	glBindTexture(GL_TEXTURE_2D, mapColourTexture);
	glUniform1i(glGetUniformLocation(textureShader->GetProgramID(), "diffuseTex"), 0);

	//BindMesh(minimapStencilQuad);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//glStencilFunc(GL_EQUAL, 2, ~0);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	BindMesh(fullScreenQuad);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_STENCIL_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void NCL::CSC8503::GameTechRenderer::DrawMinimap(){
	if (!gameWorld->GetMinimapCamera()) return;
	glBindFramebuffer(GL_FRAMEBUFFER, minimapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, minimapColourTexture, 0);

	glEnable(GL_CULL_FACE);
	glClearColor(1, 1, 1, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	Matrix4 viewMatrix = gameWorld->GetMinimapCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld->GetMinimapCamera()->BuildProjectionMatrix(screenAspect);
	RenderMaps(minimapShader, viewMatrix, projMatrix);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void NCL::CSC8503::GameTechRenderer::DrawMap(){
	if (!gameWorld->GetMapCamera()/* || !gameWorld->DoesMapNeedChecking()*/) return;
	glBindFramebuffer(GL_FRAMEBUFFER, mapFBO);

	glEnable(GL_CULL_FACE);
	glClearColor(1, 1, 1, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	float screenAspect = (float)windowWidth / (float)windowHeight;
	Matrix4 viewMatrix = gameWorld->GetMapCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld->GetMapCamera()->BuildProjectionMatrix(screenAspect);

	OGLShader* shader = mapInitShader;
	BindShader(shader);
	for (const auto& i : activeObjects) {
		if ((*i).GetRenderObject() == nullptr ) continue;

		int projLocation = glGetUniformLocation(shader->GetProgramID(), "projMatrix");
		int viewLocation = glGetUniformLocation(shader->GetProgramID(), "viewMatrix");
		int modelLocation = glGetUniformLocation(shader->GetProgramID(), "modelMatrix");
		int minimapColourLocation = glGetUniformLocation(shader->GetProgramID(), "objectMinimapColour");

		glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "team1Colour"), 1, teamColours[0].array);
		glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "team2Colour"), 1, teamColours[1].array);
		glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "team3Colour"), 1, teamColours[2].array);
		glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "team4Colour"), 1, teamColours[3].array);
		

		glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
		glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

		Matrix4 modelMatrix = (*i).GetModelMatrix();
		glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

		glUniform4fv(minimapColourLocation, 1, i->GetRenderObject()->GetMinimapColour().array);

		(*i).Draw(*this, shader == minimapShader && (*i).GetRenderObject()->GetMinimapMesh() != nullptr);
	}


	mapInitialised = true;
	updateScorebar = true;
	mapNeedsDrawing = false;
}

void GameTechRenderer::UpdateMap() {
	if (!gameWorld->GetMapCamera() || !mapInitialised) return;

	if (gameWorld->GetHitSpheres().size() > 0) {
		updateScorebar = true;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, mapFBO);
	
	float screenAspect = (float)windowWidth / (float)windowHeight;
	Matrix4 viewMatrix = gameWorld->GetMapCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld->GetMapCamera()->BuildProjectionMatrix(screenAspect);

	OGLShader* shader = mapUpdateShader;
	BindShader(mapUpdateShader);

	

	for (const auto& i : gameWorld->GetHitSpheres()) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mapColourTexture);
		glUniform1i(glGetUniformLocation(mapUpdateShader->GetProgramID(), "mapTex"), 0);

		int paintLocation = glGetUniformLocation(mapUpdateShader->GetProgramID(), "objectColour");
		glUniform3fv(paintLocation, 1, i->GetTeamColour().array);

		int atomicLocation = glGetUniformLocation(mapUpdateShader->GetProgramID(), "currentAtomicTarget");
		glUniform1i(atomicLocation, currentAtomicGPU);
		
		int screenSizeLocation = glGetUniformLocation(mapUpdateShader->GetProgramID(), "screenSize");
		Vector2 screenSize(windowWidth, windowHeight);
		glUniform2fv(screenSizeLocation, 1,  screenSize.array);

		glUniform3fv(glGetUniformLocation(mapUpdateShader->GetProgramID(), "team1Colour"), 1, teamColours[0].array);
		glUniform3fv(glGetUniformLocation(mapUpdateShader->GetProgramID(), "team2Colour"), 1, teamColours[1].array);
		glUniform3fv(glGetUniformLocation(mapUpdateShader->GetProgramID(), "team3Colour"), 1, teamColours[2].array);
		glUniform3fv(glGetUniformLocation(mapUpdateShader->GetProgramID(), "team4Colour"), 1, teamColours[3].array);
		
		int projLocation = glGetUniformLocation(mapUpdateShader->GetProgramID(), "projMatrix");
		int viewLocation = glGetUniformLocation(mapUpdateShader->GetProgramID(), "viewMatrix");

		glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
		glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

		int viewProjLocation = glGetUniformLocation(mapUpdateShader->GetProgramID(), "viewProj");
		Matrix4 viewProjectionMatrix = viewMatrix * projMatrix;
		glUniformMatrix4fv(viewProjLocation, 1, false, (float*)&viewProjectionMatrix);

		int modelLocation = glGetUniformLocation(mapUpdateShader->GetProgramID(), "modelMatrix");
		Matrix4 modelMatrix = (*i).GetModelMatrixNoRotation();
		glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

		i->CheckDrawn();

		(*i).Draw(*this);
	}

	
	

	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void NCL::CSC8503::GameTechRenderer::DrawScoreBar() {
	BindShader(scoreBarShader);

	if (updateScorebar) {
		RetrieveAtomicValues();

		glUniform1f(glGetUniformLocation(scoreBarShader->GetProgramID(), "team1PercentageOwned"), team1Percentage);
		glUniform1f(glGetUniformLocation(scoreBarShader->GetProgramID(), "team2PercentageOwned"), team2Percentage);
		glUniform1f(glGetUniformLocation(scoreBarShader->GetProgramID(), "team3PercentageOwned"), team3Percentage);
		glUniform1f(glGetUniformLocation(scoreBarShader->GetProgramID(), "team4PercentageOwned"), team4Percentage);

		glUniform3fv(glGetUniformLocation(scoreBarShader->GetProgramID(), "defaultGray"), 1, defaultColour.array);
		glUniform3fv(glGetUniformLocation(scoreBarShader->GetProgramID(), "team1Colour"), 1, teamColours[0].array);
		glUniform3fv(glGetUniformLocation(scoreBarShader->GetProgramID(), "team2Colour"), 1, teamColours[1].array);
		glUniform3fv(glGetUniformLocation(scoreBarShader->GetProgramID(), "team3Colour"), 1, teamColours[2].array);
		glUniform3fv(glGetUniformLocation(scoreBarShader->GetProgramID(), "team4Colour"), 1, teamColours[3].array);

		updateScorebar = false;
	}
	
	Matrix4 identityMatrix = Matrix4();

	int projLocation = glGetUniformLocation(scoreBarShader->GetProgramID(), "projMatrix");
	int viewLocation = glGetUniformLocation(scoreBarShader->GetProgramID(), "viewMatrix");
	int modelLocation = glGetUniformLocation(scoreBarShader->GetProgramID(), "modelMatrix");

	glUniformMatrix4fv(modelLocation, 1, false, (float*)&identityMatrix);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&identityMatrix);
	glUniformMatrix4fv(projLocation, 1, false, (float*)&identityMatrix);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	Matrix4 scoreBarModelMatrix = Matrix4::Translation(Vector3(0, 0.85f, 0)) * Matrix4::Scale(Vector3(0.4f, 0.035f, 1));
	glUniformMatrix4fv(modelLocation, 1, false, (float*)&scoreBarModelMatrix);

	BindMesh(scoreQuad);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void GameTechRenderer::RenderShadowMap() {
	
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glViewport(0, 0, shadowSize, shadowSize);

	glCullFace(GL_FRONT);

	BindShader(shadowShader);
	int mvpLocation = glGetUniformLocation(shadowShader->GetProgramID(), "mvpMatrix");
	int hasSkinLocation = glGetUniformLocation(shadowShader->GetProgramID(), "hasSkin");

	Matrix4 shadowViewMatrix = Matrix4::BuildViewMatrix(Vector3(1, 55, 1), Vector3(0, 0, 0), Vector3(0, 1, 0)); // Vector3(50, 100, 75)
	Matrix4 shadowProjMatrix = Matrix4::Perspective(20.0f, 75.0f, 1, 100.0f);

	Matrix4 mvMatrix = shadowProjMatrix * shadowViewMatrix;

	shadowMatrix = biasMatrix * mvMatrix;

	for (const auto& i : activeObjects)
	{
		Matrix4 modelMatrix = (*i).GetModelMatrix();
		Matrix4 mvpMatrix = mvMatrix * modelMatrix;
		glUniformMatrix4fv(mvpLocation, 1, false, (float*)&mvpMatrix);
		glUniform1i(hasSkinLocation, (*i).HasSkin());

		(*i).Draw(*this);
	}
	if (*currentFBO == splitFBO[0] || *currentFBO == splitFBO[1]) {
		glViewport(0, 0, windowWidth / 2, windowHeight);
	}
	else if (*currentFBO == sceneFBO) {
		glViewport(0, 0, windowWidth, windowHeight);
	}
	else {
		glViewport(0, 0, windowWidth / 2, windowHeight / 2);
	}
	
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, *currentFBO);

	glCullFace(GL_BACK);
	
}

void NCL::CSC8503::GameTechRenderer::Present1Player()
{
	PresentGameScene();
	PresentMinimap();
}
void NCL::CSC8503::GameTechRenderer::Present2Player()
{
	for (int i = 0; i < 2; i++)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, splitColourTexture[i]);
		glUniform1i(glGetUniformLocation(textureShader->GetProgramID(), "diffuseTex"), 0);

		Matrix4 modelMatrix = Matrix4::Translation(Vector3(-0.5f + i, 0, 0)) * Matrix4::Scale(Vector3(0.5, 1.0, 1.0));
		int modelLocation = glGetUniformLocation(textureShader->GetProgramID(), "modelMatrix");
		glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);
		BindMesh(fullScreenQuad);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		
	}
}
void NCL::CSC8503::GameTechRenderer::Present3Player()
{
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, quadColourTexture[0]);
	glUniform1i(glGetUniformLocation(textureShader->GetProgramID(), "diffuseTex"), 0);

	Matrix4 modelMatrix = Matrix4::Translation(Vector3(0, 0.5f, 0)) * Matrix4::Scale(Vector3(0.5, 0.5, 1.0));
	int modelLocation = glGetUniformLocation(textureShader->GetProgramID(), "modelMatrix");
	glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);
	BindMesh(fullScreenQuad);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	for (int i = 0; i < 2; i++)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, quadColourTexture[i + 1]);
		glUniform1i(glGetUniformLocation(textureShader->GetProgramID(), "diffuseTex"), 0);

		Matrix4 modelMatrix = Matrix4::Translation(Vector3(0.5f - i, -0.5f, 0)) * Matrix4::Scale(Vector3(0.5, 0.5, 1.0));
		int modelLocation = glGetUniformLocation(textureShader->GetProgramID(), "modelMatrix");
		glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);
		BindMesh(fullScreenQuad);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	

}
void NCL::CSC8503::GameTechRenderer::Present4Player()
{

	Vector3 translations[4] = { 
		Vector3(-0.5, 0.5, 0.0),  // Player 1
		Vector3(0.5, 0.5, 0.0),   // Player 2
		Vector3(-0.5, -0.5, 0.0), // Player 3
		Vector3(0.5, -0.5, 0.0)   // Player 4
	};
	for (int i = 0; i < 4; i++)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, quadColourTexture[i]);
		glUniform1i(glGetUniformLocation(textureShader->GetProgramID(), "diffuseTex"), 0);
		Matrix4 modelMatrix = Matrix4::Translation(translations[i]) * Matrix4::Scale(Vector3(0.5, 0.5, 1.0));
		int modelLocation = glGetUniformLocation(textureShader->GetProgramID(), "modelMatrix");
		glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);
		BindMesh(fullScreenQuad);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void GameTechRenderer::RenderSkybox(bool enableTests) {
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	float screenAspect = (float)windowWidth / (float)windowHeight;
	Matrix4 viewMatrix = gameWorld ? currentRenderCamera->BuildViewMatrix() : Matrix4();
	Matrix4 projMatrix = gameWorld ? currentRenderCamera->BuildProjectionMatrix(screenAspect) : Matrix4();

	BindShader(skyboxShader);

	int projLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "projMatrix");
	int viewLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "viewMatrix");
	int texLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "cubeTex");

	glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

	glUniform1i(texLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	BindMesh(skyboxMesh);
	DrawBoundMesh();
	if (enableTests) {
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
}

void GameTechRenderer::LoadSkybox(string fileName) {
	// TODO: Move the texture loading into TAM? Not sure if possible without major refactor
	string filenames[6] = {
		fileName.empty() ? "/Cubemap/skyrender0004.png" : Assets::TEXTUREDIR + fileName,
		fileName.empty() ? "/Cubemap/skyrender0001.png"  : Assets::TEXTUREDIR + fileName,
		fileName.empty() ? "/Cubemap/skyrender0003.png"  : Assets::TEXTUREDIR + fileName,
		fileName.empty() ? "/Cubemap/skyrender0006.png"  : Assets::TEXTUREDIR + fileName,
		fileName.empty() ? "/Cubemap/skyrender0002.png"  : Assets::TEXTUREDIR + fileName,
		fileName.empty() ? "/Cubemap/skyrender0005.png"  : Assets::TEXTUREDIR + fileName
	};

	int width[6] = { 0 };
	int height[6] = { 0 };
	int channels[6] = { 0 };
	int flags[6] = { 0 };

	vector<char*> texData(6, nullptr);

	if (!fileName.empty()) {
		TextureLoader::LoadTexture(filenames[0], texData[0], width[0], height[0], channels[0], flags[0]);
		for (int i = 0; i < 6; i++) {
			texData[i] = texData[0];
			width[i] = width[0];
			height[i] = height[0];
			channels[i] = channels[0];
			flags[i] = flags[0];
		}
	}
	else for (int i = 0; i < 6; ++i) {
		TextureLoader::LoadTexture(filenames[i], texData[i], width[i], height[i], channels[i], flags[i]);
		if (i > 0 && (width[i] != width[0] || height[0] != height[0])) {
			std::cout << __FUNCTION__ << " cubemap input textures don't match in size?\n";
			return;
		}
	}
	glGenTextures(1, &skyboxTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	GLenum type = channels[0] == 4 ? GL_RGBA : GL_RGB;

	for (int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width[i], height[i], 0, type, GL_UNSIGNED_BYTE, texData[i]);
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void GameTechRenderer::RenderFrameLoading() {
	BeginFrame();
	RenderSkybox(false);
	DrawLoader();
	NewRenderLines();
	NewRenderLinesOnOrthographicView();
	NewRenderText();
	Debug::UpdateRenderables(0.1f);
	EndFrame();
	SwapBuffers();
}

void NCL::CSC8503::GameTechRenderer::RenderImGUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Debug Window");
	if (ImGui::CollapsingHeader("Camera"))
	{
		ToonFollowCamera* followCamera = (ToonFollowCamera*)(gameWorld->GetMainCamera(1));

		Vector3 cPos = gameWorld->GetMainCamera(1)->GetPosition();
		Vector3 cRot(gameWorld->GetMainCamera(1)->GetPitch(), gameWorld->GetMainCamera(1)->GetYaw(), 0);
		Vector3 cFollowOffset = followCamera->GetFollowOffset();
		Vector3 cFollowOffset2 = followCamera->followOffset2;
		Vector3 cTargetOffset = followCamera->GetTargetOffset();
		Vector3 cAimOffset = followCamera->GetAimOffset();

		float distance = followCamera->GetFollowDistance();
		float smoothness = followCamera->GetSmoothness();
		float cPitchOffset = followCamera->GetPitchOffset();

		if (ImGui::DragFloat3("Cam Position", (float*)&cPos)) gameWorld->GetMainCamera(1)->SetPosition(cPos);
		if (ImGui::DragFloat("Cam Pitch", (float*)&cRot.x)) gameWorld->GetMainCamera(1)->SetPitch(cPos.x);
		if (ImGui::DragFloat("Cam Yaw", (float*)&cRot.y)) gameWorld->GetMainCamera(1)->SetYaw(cPos.y);
		if (ImGui::DragFloat("Pitch Offset", (float*)&cPitchOffset)) followCamera->SetPitchOffset(cPitchOffset);

		if (ImGui::DragFloat("Follow Distance", (float*)&distance)) followCamera->SetFollowDistance(distance);
		if (ImGui::DragFloat("Follow Smoothness", (float*)&smoothness)) followCamera->SetSmoothness(smoothness);
		if (ImGui::DragFloat3("Follow Offset", (float*)&cFollowOffset)) followCamera->SetFollowOffset(cFollowOffset);
		if (ImGui::DragFloat3("Follow Offset 2", (float*)&cFollowOffset2)) followCamera->followOffset2 = cFollowOffset2;
		if (ImGui::DragFloat3("Target Offset", (float*)&cTargetOffset)) followCamera->SetTargetOffset(cTargetOffset);
		if (ImGui::DragFloat3("Aim Offset", (float*)&cAimOffset)) followCamera->SetAimOffset(cAimOffset);
	}

	ImGui::End();
	if (ImGui::Begin("Performance Window")) {
		ImGui::BeginTable("FPS Table", 2);
		ImGui::TableNextColumn();
		ImGui::Text("FPS");
		ImGui::TableNextColumn();
		ImGui::Text(std::to_string(ImGui::GetIO().Framerate).c_str());
		ImGui::EndTable();

		if (ImGui::CollapsingHeader("Memory Usage")) {
			ImGui::BeginTable("Memory Usage Table", 2);

			ImGui::TableNextColumn();
			ImGui::Text("Virtual Memory");
			ImGui::TableNextColumn();
			ImGui::Text(ToonDebugManager::Instance().GetVirtualMemoryUsage().c_str());

			ImGui::TableNextColumn();

			ImGui::Text("Virtual Memory By Program");
			ImGui::TableNextColumn();
			ImGui::Text(ToonDebugManager::Instance().GetVirutalUsageByProgram().c_str());

			ImGui::TableNextColumn();

			ImGui::Text("Physcial Memory");
			ImGui::TableNextColumn();
			ImGui::Text(ToonDebugManager::Instance().GetPhysicalMemoryUsage().c_str());

			ImGui::TableNextColumn();

			ImGui::Text("Physcial Memory By Program");
			ImGui::TableNextColumn();
			ImGui::Text(ToonDebugManager::Instance().GetPhysicalUsagebyProgram().c_str());

			ImGui::EndTable();

		}
		if (ImGui::CollapsingHeader("Update Times")) {
			ImGui::BeginTable("Update Table", 2);

			ImGui::TableNextColumn();

			ImGui::Text("Load Time");
			ImGui::TableNextColumn();
			ImGui::Text(ToonDebugManager::Instance().GetLoadTimeTaken().c_str());
			ImGui::TableNextColumn();

			ImGui::Text("Frame Time");
			ImGui::TableNextColumn();
			ImGui::Text(ToonDebugManager::Instance().GetFrameTimeTaken().c_str());
			ImGui::TableNextColumn();


			ImGui::Text("Audio Time");
			ImGui::TableNextColumn();
			ImGui::Text(ToonDebugManager::Instance().GetAudioTimeTaken().c_str());
			ImGui::TableNextColumn();

			ImGui::Text("Networking Time");
			ImGui::TableNextColumn();
			ImGui::Text(ToonDebugManager::Instance().GetNetworkingTimeTaken().c_str());
			ImGui::TableNextColumn();

			ImGui::Text("Physics Time");
			ImGui::TableNextColumn();
			ImGui::Text(ToonDebugManager::Instance().GetPhysicsTimeTaken().c_str());
			ImGui::TableNextColumn();

			ImGui::Text("Animation Time");
			ImGui::TableNextColumn();
			ImGui::Text(ToonDebugManager::Instance().GetAnimationTimeTaken().c_str());
			ImGui::TableNextColumn();

			ImGui::Text("Graphics Time");
			ImGui::TableNextColumn();
			ImGui::Text(ToonDebugManager::Instance().GetGraphicsTimeTaken().c_str());
			ImGui::EndTable();
		}
	}
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GameTechRenderer::BuildObjectList() {
	activeObjects.clear();

	gameWorld->OperateOnContents(
		[&](ToonGameObject* o) 
		{
			if (o->IsActive()) 
			{
				o->CalculateModelMatrix();
				activeObjects.emplace_back(o);
			}
		}
	);
}

void NCL::CSC8503::GameTechRenderer::CalculatePercentages(const int& totalPixels, const int& team1Pixels, const int& team2Pixels, const int& team3Pixels, const int& team4Pixels) {
	float totalPaintedPixels = (float)team1Pixels + (float)team2Pixels + (float)team3Pixels + (float)team4Pixels;
	if (totalPaintedPixels != 0) {
		team1Percentage = (float)team1Pixels / totalPaintedPixels;
		team2Percentage = (float)team2Pixels / totalPaintedPixels;
		team3Percentage = (float)team3Pixels / totalPaintedPixels;
		team4Percentage = (float)team4Pixels / totalPaintedPixels;
	}
	else {
		team1Percentage = 0;
		team2Percentage = 0;
		team3Percentage = 0;
		team4Percentage = 0;
	}
}

void GameTechRenderer::SortObjectList() {

}

void GameTechRenderer::PassImpactPointDetails(ToonGameObject* const& paintedObject, OGLShader* shader)
{
	int impactPointsLocation = 0;
	int impactPointCountLocation = glGetUniformLocation(shader->GetProgramID(), "impactPointCount");

	int isFloorLocation = glGetUniformLocation(shader->GetProgramID(), "isFloor");

	std::deque<ImpactPoint>* objImpactPoints;
	if (dynamic_cast<PaintableObject*>(paintedObject)) {
		PaintableObject* object = (PaintableObject*)paintedObject;
		glUniform1i(isFloorLocation, object->IsObjectTheFloor() ? 1 : 0);

		objImpactPoints = object->GetImpactPoints();
	}
	else {
		Player* object = (Player*)paintedObject;
		glUniform1i(isFloorLocation, object->IsObjectTheFloor() ? 1 : 0);

		objImpactPoints = object->GetImpactPoints();
	}

	glUniform1i(impactPointCountLocation, (GLint)objImpactPoints->size());

	if (objImpactPoints->empty()) return;

	GLuint i = 0;
	for (ImpactPoint& point : *objImpactPoints) {
		char buffer[64];

		sprintf_s(buffer, "impactPoints[%i].position", i);
		impactPointsLocation = glGetUniformLocation(shader->GetProgramID(), buffer);
		Vector3 impactLocation = point.GetImpactLocation();
		glUniform3fv(impactPointsLocation, 1, (float*)&impactLocation);

		sprintf_s(buffer, "impactPoints[%i].colour", i);
		impactPointsLocation = glGetUniformLocation(shader->GetProgramID(), buffer);
		Vector3 impactColour = point.GetImpactColour();
		glUniform3fv(impactPointsLocation, 1, (float*)&impactColour);

		sprintf_s(buffer, "impactPoints[%i].radius", i);
		impactPointsLocation = glGetUniformLocation(shader->GetProgramID(), buffer);
		glUniform1f(impactPointsLocation, point.GetImpactRadius());

		i++;
	}
}

void GameTechRenderer::NewRenderLines() {
	const std::vector<Debug::DebugLineEntry>& lines = Debug::GetDebugLines();
	if (lines.empty()) {
		return;
	}
	float screenAspect = (float)windowWidth / (float)windowHeight;
	Matrix4 viewMatrix = gameWorld ? gameWorld->GetMainCamera(1)->BuildViewMatrix() : Matrix4();
	Matrix4 projMatrix = gameWorld ? gameWorld->GetMainCamera(1)->BuildProjectionMatrix(screenAspect) : Matrix4();

	Matrix4 viewProj = projMatrix * viewMatrix;

	BindShader(debugShader);
	int matSlot = glGetUniformLocation(debugShader->GetProgramID(), "viewProjMatrix");
	GLuint texSlot = glGetUniformLocation(debugShader->GetProgramID(), "useTexture");
	glUniform1i(texSlot, 0);

	glUniformMatrix4fv(matSlot, 1, false, (float*)viewProj.array);

	debugLineData.clear();

	int frameLineCount = (int)lines.size() * 2;

	SetDebugLineBufferSizes(frameLineCount);

	glBindBuffer(GL_ARRAY_BUFFER, lineVertVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, lines.size() * sizeof(Debug::DebugLineEntry), lines.data());

	glBindVertexArray(lineVAO);
	glDrawArrays(GL_LINES, 0, frameLineCount);
	glBindVertexArray(0);
}

void GameTechRenderer::NewRenderLinesOnOrthographicView(){
	const std::vector<Debug::DebugLineEntry>& lines = Debug::GetOrthographicViewLines();
	if (lines.empty()) {
		return;
	}
	float screenAspect = (float)windowWidth / (float)windowHeight;

	Matrix4 proj = Matrix4::Orthographic(0.0, 100.0f, 100, 0, -1.0f, 1.0f);

	BindShader(debugShader);
	int matSlot = glGetUniformLocation(debugShader->GetProgramID(), "viewProjMatrix");
	GLuint texSlot = glGetUniformLocation(debugShader->GetProgramID(), "useTexture");
	glUniform1i(texSlot, 0);

	glUniformMatrix4fv(matSlot, 1, false, (float*)proj.array);

	debugLineData.clear();

	int frameLineCount = (int)lines.size() * 2;

	SetDebugLineBufferSizes(frameLineCount);

	glBindBuffer(GL_ARRAY_BUFFER, lineVertVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, lines.size() * sizeof(Debug::DebugLineEntry), lines.data());

	glBindVertexArray(lineVAO);
	glDrawArrays(GL_LINES, 0, frameLineCount);
	glBindVertexArray(0);
}

void GameTechRenderer::NewRenderText() {
	const std::vector<Debug::DebugStringEntry>& strings = Debug::GetDebugStrings();
	if (strings.empty()) {
		return;
	}

	BindShader(debugShader);

	OGLTexture* t = (OGLTexture*)Debug::GetDebugFont()->GetTexture();

	if (t) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t->GetObjectID());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		BindTextureToShader(t, "mainTex", 0);
	}
	Matrix4 proj = Matrix4::Orthographic(0.0, 100.0f, 100, 0, -1.0f, 1.0f);

	int matSlot = glGetUniformLocation(debugShader->GetProgramID(), "viewProjMatrix");
	glUniformMatrix4fv(matSlot, 1, false, (float*)proj.array);

	GLuint texSlot = glGetUniformLocation(debugShader->GetProgramID(), "useTexture");
	glUniform1i(texSlot, 1);

	debugTextPos.clear();
	debugTextColours.clear();
	debugTextUVs.clear();

	int frameVertCount = 0;
	for (const auto& s : strings) {
		frameVertCount += Debug::GetDebugFont()->GetVertexCountForString(s.data);
	}
	SetDebugStringBufferSizes(frameVertCount);

	for (const auto& s : strings) {
		float size = 20.0f;
		Debug::GetDebugFont()->BuildVerticesForString(s.data, s.position, s.colour, size, debugTextPos, debugTextUVs, debugTextColours);
	}

	glBindBuffer(GL_ARRAY_BUFFER, textVertVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, frameVertCount * sizeof(Vector3), debugTextPos.data());
	glBindBuffer(GL_ARRAY_BUFFER, textColourVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, frameVertCount * sizeof(Vector4), debugTextColours.data());
	glBindBuffer(GL_ARRAY_BUFFER, textTexVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, frameVertCount * sizeof(Vector2), debugTextUVs.data());

	glBindVertexArray(textVAO);
	glDrawArrays(GL_TRIANGLES, 0, frameVertCount);
	glBindVertexArray(0);
}

void GameTechRenderer::GenerateAtomicBuffer(){
	glGenBuffers(1, &atomicsBuffer[0]);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer[0]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomicsBuffer[0]);
	glBufferStorage(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint) * ATOMIC_COUNT, NULL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT | GL_MAP_COHERENT_BIT);

	glGenBuffers(1, &atomicsBuffer[1]);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer[1]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 1, atomicsBuffer[1]);
	glBufferStorage(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint) * ATOMIC_COUNT, NULL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT | GL_MAP_COHERENT_BIT);
	
	glGenBuffers(1, &atomicsBuffer[2]);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer[2]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, atomicsBuffer[2]);
	glBufferStorage(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint) * ATOMIC_COUNT, NULL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT | GL_MAP_COHERENT_BIT);
	
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, 0);
	
	currentAtomicCPU = 0;
	currentAtomicReset = 0;
	currentAtomicGPU = 0;
}
	
	
void GameTechRenderer::RetrieveAtomicValues(){
	GLuint pixelCount[ATOMIC_COUNT];
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer[currentAtomicCPU]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, currentAtomicCPU, atomicsBuffer[currentAtomicCPU]);

	glGetBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint) * ATOMIC_COUNT, pixelCount);

	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
	totalPixelCount = pixelCount[0];

	for (GLuint i = 1; i < ATOMIC_COUNT; i++)
	{
		teamPixelCount[i - 1] = pixelCount[i];
	}
	
	CalculatePercentages(totalPixelCount, teamPixelCount[0], teamPixelCount[1], teamPixelCount[2], teamPixelCount[3]);

	//ResetAtomicBuffer();
}

void GameTechRenderer::ResetAtomicBuffer(){
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer[currentAtomicCPU]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, currentAtomicCPU, atomicsBuffer[currentAtomicCPU]);
	GLuint a[ATOMIC_COUNT];
	for (GLuint i = 0; i < ATOMIC_COUNT; i++)
	{
		a[i] = 0;
	}
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint) * ATOMIC_COUNT, a);

	updateScorebar = true;
	mapInitialised = false;
}

int GameTechRenderer::GetWinningTeam(float& percentage) {
	std::map<int, float> scores = GetTeamScores();
	float winningPercentage = 0.0f;
	float secondPercentage = 0.0f;
	int winningTeam = 0;
	
	std::map<int, float>::iterator it;
	for (it = scores.begin(); it != scores.end(); it++) {
		if (it->second > winningPercentage) {
			secondPercentage = winningPercentage;
			winningPercentage = it->second;
			winningTeam = it->first;
		}
		else if (it->second > secondPercentage) {
			secondPercentage = it->second;
		}
		
	}

	percentage = 1.0f / (winningPercentage / secondPercentage);

	if (winningPercentage == 0) {
		percentage = 1.0f;
	}
	return winningTeam;
}

std::map<int, float> GameTechRenderer::GetTeamScores() {
	std::map<int, float> scores;
	scores.emplace(1, team1Percentage);
	scores.emplace(2, team2Percentage);
	scores.emplace(3, team3Percentage);
	scores.emplace(4, team4Percentage);
	return scores;
}


void GameTechRenderer::SetWorld(ToonGameWorld* world)
{
	gameWorld = world;
	std::map<int, Team*> teams = gameWorld->GetTeams();

	int i = 0;
	for (auto& [ID, team] : teams) {
		teamColours[i] = team->GetTeamColour();
		i++;
	}
}

void GameTechRenderer::SetDebugStringBufferSizes(size_t newVertCount) {
	if (newVertCount > textCount) {
		textCount = newVertCount;

		glBindBuffer(GL_ARRAY_BUFFER, textVertVBO);
		glBufferData(GL_ARRAY_BUFFER, textCount * sizeof(Vector3), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, textColourVBO);
		glBufferData(GL_ARRAY_BUFFER, textCount * sizeof(Vector4), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, textTexVBO);
		glBufferData(GL_ARRAY_BUFFER, textCount * sizeof(Vector2), nullptr, GL_DYNAMIC_DRAW);

		debugTextPos.reserve(textCount);
		debugTextColours.reserve(textCount);
		debugTextUVs.reserve(textCount);

		glBindVertexArray(textVAO);

		glVertexAttribFormat(0, 3, GL_FLOAT, false, 0);
		glVertexAttribBinding(0, 0);
		glBindVertexBuffer(0, textVertVBO, 0, sizeof(Vector3));

		glVertexAttribFormat(1, 4, GL_FLOAT, false, 0);
		glVertexAttribBinding(1, 1);
		glBindVertexBuffer(1, textColourVBO, 0, sizeof(Vector4));

		glVertexAttribFormat(2, 2, GL_FLOAT, false, 0);
		glVertexAttribBinding(2, 2);
		glBindVertexBuffer(2, textTexVBO, 0, sizeof(Vector2));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}
}

void GameTechRenderer::SetDebugLineBufferSizes(size_t newVertCount) {
	if (newVertCount > lineCount) {
		lineCount = newVertCount;

		glBindBuffer(GL_ARRAY_BUFFER, lineVertVBO);
		glBufferData(GL_ARRAY_BUFFER, lineCount * sizeof(Debug::DebugLineEntry), nullptr, GL_DYNAMIC_DRAW);

		debugLineData.reserve(lineCount);

		glBindVertexArray(lineVAO);

		int realStride = sizeof(Debug::DebugLineEntry) / 2;

		glVertexAttribFormat(0, 3, GL_FLOAT, false, offsetof(Debug::DebugLineEntry, start));
		glVertexAttribBinding(0, 0);
		glBindVertexBuffer(0, lineVertVBO, 0, realStride);

		glVertexAttribFormat(1, 4, GL_FLOAT, false, offsetof(Debug::DebugLineEntry, colourA));
		glVertexAttribBinding(1, 0);
		glBindVertexBuffer(1, lineVertVBO, sizeof(Vector4), realStride);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}
}

void NCL::CSC8503::GameTechRenderer::GenerateShadowFBO()
{
	if (shadowTex != 0) {
		glDeleteTextures(1, &shadowTex);
		glDeleteFramebuffers(1, &shadowFBO);
	}
	
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		shadowSize, shadowSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !shadowTex) std::cout << "Error Creating Buffer" << std::endl;

	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GameTechRenderer::GenerateSceneFBO(int width, int height)
{
	glGenFramebuffers(1, &sceneFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);

	glGenTextures(1, &sceneColourTexture);
	glBindTexture(GL_TEXTURE_2D, sceneColourTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneColourTexture, 0);
	glObjectLabel(GL_TEXTURE, sceneColourTexture, -1, "Scene Colour Texture");

	glGenTextures(1, &sceneDepthTexture);
	glBindTexture(GL_TEXTURE_2D, sceneDepthTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sceneDepthTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, sceneDepthTexture, 0);

	glObjectLabel(GL_TEXTURE, sceneDepthTexture, -1, "Scene Depth Texture");

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !sceneColourTexture || !sceneColourTexture) {
		return;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GameTechRenderer::GenerateMinimapFBO(int width, int height)
{
	glGenFramebuffers(1, &minimapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, minimapFBO);

	glGenTextures(1, &minimapColourTexture);
	glBindTexture(GL_TEXTURE_2D, minimapColourTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, minimapColourTexture, 0);
	glObjectLabel(GL_TEXTURE, minimapColourTexture, -1, "Minimap Colour Texture");

	glGenTextures(1, &minimapDepthTexture);
	glBindTexture(GL_TEXTURE_2D, minimapDepthTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, minimapDepthTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, minimapDepthTexture, 0);

	glObjectLabel(GL_TEXTURE, minimapDepthTexture, -1, "Minimap Depth Texture");

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !minimapColourTexture || !minimapColourTexture) {
		return;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void GameTechRenderer::GenerateMapFBO(int width, int height)
{
	glGenFramebuffers(1, &mapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mapFBO);

	glGenTextures(1, &mapColourTexture);
	glBindTexture(GL_TEXTURE_2D, mapColourTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mapColourTexture, 0);
	glObjectLabel(GL_TEXTURE, mapColourTexture, -1, "Mainmap Colour Texture");

	glGenTextures(1, &mapDepthTexture);
	glBindTexture(GL_TEXTURE_2D, mapDepthTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mapDepthTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mapDepthTexture, 0);

	glObjectLabel(GL_TEXTURE, mapDepthTexture, -1, "Mainmap Depth Texture");

	glGenTextures(1, &mapPositionTexture);
	glBindTexture(GL_TEXTURE_2D, mapPositionTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mapPositionTexture, 0);

	glObjectLabel(GL_TEXTURE, mapPositionTexture, -1, "Mainmap Position Texture");

	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !mapColourTexture || !mapPositionTexture) {
		return;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void NCL::CSC8503::GameTechRenderer::GenerateSplitFBO(int width, int height)
{
	for (int i = 0; i < 2; i++)
	{
		glGenFramebuffers(1, &splitFBO[i]);
		glBindFramebuffer(GL_FRAMEBUFFER, splitFBO[i]);


		glGenTextures(1, &splitColourTexture[i]);
		glBindTexture(GL_TEXTURE_2D, splitColourTexture[i]);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, splitColourTexture[i], 0);
		glObjectLabel(GL_TEXTURE, splitColourTexture[i], -1, "Split Colour Texture");

		glGenTextures(1, &splitDepthTexture[i]);
		glBindTexture(GL_TEXTURE_2D, splitDepthTexture[i]);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, splitDepthTexture[i], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, splitDepthTexture[i], 0);
		glObjectLabel(GL_TEXTURE, splitDepthTexture[i], -1, "Split Depth Texture");

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !splitDepthTexture[i] || !splitColourTexture[i]) {
			return;
		}
	}



	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void NCL::CSC8503::GameTechRenderer::CreateLightUBO() {
	glGenBuffers(1, &lightMatrix);
	glBindBuffer(GL_UNIFORM_BUFFER, lightMatrix);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightStruct), &shaderLight, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	unsigned int sceneIndex = glGetUniformBlockIndex(sceneShader->GetProgramID(), "lights");

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightMatrix);
	glUniformBlockBinding(sceneShader->GetProgramID(), sceneIndex, 1);
}

void NCL::CSC8503::GameTechRenderer::GenerateQuadFBO(int width, int height)
{
	for (int i = 0; i < 4; i++)
	{
		glGenFramebuffers(1, &quadFBO[i]);
		glBindFramebuffer(GL_FRAMEBUFFER, quadFBO[i]);


		glGenTextures(1, &quadColourTexture[i]);
		glBindTexture(GL_TEXTURE_2D, quadColourTexture[i]);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, quadColourTexture[i], 0);
		glObjectLabel(GL_TEXTURE, quadColourTexture[i], -1, "quad Colour Texture");

		glGenTextures(1, &quadDepthTexture[i]);
		glBindTexture(GL_TEXTURE_2D, quadDepthTexture[i]);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, quadDepthTexture[i], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, quadDepthTexture[i], 0);
		glObjectLabel(GL_TEXTURE, quadDepthTexture[i], -1, "Split Depth Texture");

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !quadDepthTexture[i] || !quadColourTexture[i]) {
			return;
		}
	}



	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void NCL::CSC8503::GameTechRenderer::DrawLoader()
{
	Vector2		position = Vector2(10.0f, 80.0f);
	const float width = 50.0f;
	const float height = 5.0f;

	NCL::ToonAssetManager::LoadingDataStructure loadingData = ToonAssetManager::Instance().loadingData;

	Debug::DrawQuad(position, Vector2(width, height), Debug::GREEN);
	Debug::DrawFilledQuad(position, Vector2(loadingData.assetCountDone * (width / loadingData.assetCountTotal), height), 100.0f/windowHeight, Debug::GREEN);
	Debug::Print("Loading " + loadingData.loadingText + " (" + std::to_string(loadingData.assetCountDone) + "/" + std::to_string(loadingData.assetCountTotal) + ")", position + Vector2(0.0f, (2 * height)), Debug::GREEN);
}