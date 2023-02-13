#include "GameTechRenderer.h"
#include "GameObject.h"
#include "ToonGameObject.h"
#include "RenderObject.h"
#include "ToonRenderObject.h"
#include "ToonFollowCamera.h"
#include "ToonLevelManager.h"
#include "ToonUtils.h"
#include "TextureLoader.h"

#include "ImpactPoint.h"
#include "PaintableObject.h"
#include "ToonUtils.h"

#include "../ThirdParty/imgui/imgui.h"
#include "../ThirdParty/imgui/imgui_impl_opengl3.h"
#include "../ThirdParty/imgui/imgui_impl_win32.h"

#include "Light.h"


using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

#define SHADOWSIZE 4096 

Matrix4 biasMatrix = Matrix4::Translation(NCL::Maths::Vector3(0.5f, 0.5f, 0.5f)) * Matrix4::Scale(NCL::Maths::Vector3(0.5f, 0.5f, 0.5f));
ToonFollowCamera* followCamera;

GameTechRenderer::GameTechRenderer(ToonGameWorld& world) : OGLRenderer(*Window::GetWindow()), gameWorld(world)	
{	
	SetupStuffs();
}

GameTechRenderer::~GameTechRenderer()	{
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
}

void NCL::CSC8503::GameTechRenderer::SetupStuffs()
{
	glEnable(GL_DEPTH_TEST);

	debugShader = new OGLShader("debug.vert", "debug.frag");

	
	debugShader  = new OGLShader("debug.vert", "debug.frag");

	shadowShader = new OGLShader("shadow.vert", "shadow.frag");
	minimapShader = new OGLShader("minimap.vert", "minimap.frag");
	textureShader = new OGLShader("Texture.vert", "Texture.frag");
	sceneShader = new OGLShader("scene.vert", "scene.frag");

	GenerateShadowFBO();
	GenerateSceneFBO(windowWidth, windowHeight);
	GenerateMinimapFBO(windowWidth, windowHeight);

	glClearColor(1, 1, 1, 1);
	
	//Set up the light properties
	lightColour = Vector4(0.8f, 0.8f, 0.5f, 1.0f);
	lightRadius = 1000.0f;
	lightPosition = Vector3(-200.0f, 60.0f, -200.0f);

	//Skybox!
	skyboxShader = new OGLShader("skybox.vert", "skybox.frag");
	skyboxMesh = new OGLMesh();
	skyboxMesh->SetVertexPositions({ Vector3(-1, 1,-1), Vector3(-1,-1,-1) , Vector3(1,-1,-1) , Vector3(1,1,-1) });
	skyboxMesh->SetVertexIndices({ 0,1,2,2,3,0 });
	skyboxMesh->UploadToGPU();


	fullScreenQuad = new OGLMesh();
	fullScreenQuad->SetVertexPositions({ Vector3(-1, 1,1), Vector3(-1,-1,1) , Vector3(1,-1,1) , Vector3(1,1,1) });
	fullScreenQuad->SetVertexTextureCoords({ Vector2(0.0f,1.0f), Vector2(0.0f,0.0f), Vector2(1.0f,0.0f), Vector2(1.0f,1.0f) });
	fullScreenQuad->SetVertexIndices({ 0,1,2,2,3,0 });
	fullScreenQuad->UploadToGPU();
	
	minimapQuad = new OGLMesh();
	minimapQuad->SetVertexPositions({ Vector3(-1, 1,-1), Vector3(-1,-1,-1) , Vector3(1,-1,-1) , Vector3(1,1,-1) });
	minimapQuad->SetVertexTextureCoords({ Vector2(0.0f,1.0f), Vector2(0.0f,0.0f), Vector2(1.0f,0.0f), Vector2(1.0f,1.0f) });
	minimapQuad->SetVertexIndices({ 0,1,2,2,3,0 });
	minimapQuad->UploadToGPU();

	minimapStencilQuad = new OGLMesh();
	minimapStencilQuad->SetVertexPositions({ Vector3(-0.5f, 0.8f, -1.0f), Vector3(-0.5f, -0.8f, -1.0f) , Vector3(0.5f, -0.8f, -1.0f) , Vector3(0.5f, 0.8f, -1.0f) });
	minimapStencilQuad->SetVertexIndices({ 0,1,2,2,3,0 });
	minimapStencilQuad->UploadToGPU();
	


LoadDeferedLighting();

	LoadSkybox();

	glGenVertexArrays(1, &lineVAO);
	glGenVertexArrays(1, &textVAO);

	glGenBuffers(1, &lineVertVBO);
	glGenBuffers(1, &textVertVBO);
	glGenBuffers(1, &textColourVBO);
	glGenBuffers(1, &textTexVBO);

	SetDebugStringBufferSizes(10000);
	SetDebugLineBufferSizes(1000);
}


void NCL::CSC8503::GameTechRenderer::GenerateShadowFBO()
{
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GameTechRenderer::GenerateSceneFBO(int width, int height)
{
	glGenFramebuffers(1, &sceneFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);

	glGenTextures(1, &sceneColourTexture);
	glBindTexture(GL_TEXTURE_2D, sceneColourTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneColourTexture, 0);
	glObjectLabel(GL_TEXTURE, sceneColourTexture, -1, "Scene Colour Texture");

	glGenTextures(1, &sceneDepthTexture);
	glBindTexture(GL_TEXTURE_2D, sceneDepthTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
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

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, minimapColourTexture, 0);
	glObjectLabel(GL_TEXTURE, minimapColourTexture, -1, "Minimap Texture");

	glGenTextures(1, &minimapDepthTexture);
	glBindTexture(GL_TEXTURE_2D, minimapDepthTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, minimapDepthTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, minimapDepthTexture, 0);

	glObjectLabel(GL_TEXTURE, minimapDepthTexture, -1, "Scene Depth Texture");

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !minimapColourTexture || !minimapColourTexture) {
		return;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

GameTechRenderer::~GameTechRenderer() {
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
	delete SceneShader;
	delete CombineShader;
	delete PointLightShader;

	
	delete sphere;
	delete quad;
	delete[] pointLights;
	glDeleteTextures(1, &bufferColourTex);
	glDeleteTextures(1, &bufferNormalTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteTextures(1, &lightDiffuseTex);
	glDeleteTextures(1, &lightSpecularTex);

	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &pointLightFBO);

}

void GameTechRenderer::LoadDeferedLighting() {
	bool init = false;
	const int LIGHT_NUM = 24;
	pointLights = new Light[LIGHT_NUM];

	for (int i = 0; i < LIGHT_NUM; ++i) {
		Light& l = pointLights[i];
		l.SetPosition(Vector3(rand() % 10,
			150.0f,
			rand() % (int)10));

		l.SetColour(Vector4(0.5f + (float)(rand() / (float)RAND_MAX),
			0.5f + (float)(rand() / (float)RAND_MAX),
			0.5f + (float)(rand() / (float)RAND_MAX),
			1));
		l.SetRadius(250.0f + (rand() % 250));
	}
	//SceneShader = new OGLShader("BumpVertex.glsl", // reused!
	//	"bufferFragment.glsl");
	PointLightShader = new OGLShader("pointlightvertex.vert",
		"pointlightfrag.glsl");
	CombineShader = new OGLShader("CombineVert.vert",
		"CombineFrag.frag");

	if ( !PointLightShader->LoadSuccess() ||
		!CombineShader->LoadSuccess()) {
		return;

	}
	glGenFramebuffers(1, &bufferFBO);
	glGenFramebuffers(1, &pointLightFBO);

	GLenum buffers[2] = {
	GL_COLOR_ATTACHMENT0 ,
	GL_COLOR_ATTACHMENT1
	};
	GenerateScreenTexture(bufferDepthTex, true);
	GenerateScreenTexture(bufferColourTex, false);
	GenerateScreenTexture(bufferNormalTex,false);
	GenerateScreenTexture(lightDiffuseTex,false);
	GenerateScreenTexture(lightSpecularTex,false);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, bufferNormalTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, bufferDepthTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE) {
		return;

	}
	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, lightDiffuseTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, lightSpecularTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE) {
		return;

	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	init = true;


}


void GameTechRenderer::LoadSkybox() {
	string filenames[6] = {
		"/Cubemap/skyrender0004.png",
		"/Cubemap/skyrender0001.png",
		"/Cubemap/skyrender0003.png",
		"/Cubemap/skyrender0006.png",
		"/Cubemap/skyrender0002.png",
		"/Cubemap/skyrender0005.png"
	};

	int width[6] = { 0 };
	int height[6] = { 0 };
	int channels[6] = { 0 };
	int flags[6] = { 0 };

	vector<char*> texData(6, nullptr);

	for (int i = 0; i < 6; ++i) {
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

void GameTechRenderer::RenderFrame() {
	

	DrawMainScene();
	if (minimapEnabled)
	{
		
		DrawMinimap();
	}
	PresentScene();
	
	
	RenderImGUI();

}




void NCL::CSC8503::GameTechRenderer::DrawMainScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneColourTexture, 0);
	glEnable(GL_CULL_FACE);
	glClearColor(1, 1, 1, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	BuildObjectList();
	SortObjectList();
	RenderShadowMap();
	RenderSkybox();
	DrawPointLights();
	CombineBuffers();
	//RenderCamera();
	glDisable(GL_CULL_FACE); //Todo - text indices are going the wrong way...
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	NewRenderLines();
	NewRenderText();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void NCL::CSC8503::GameTechRenderer::DrawMinimap()
{
	glBindFramebuffer(GL_FRAMEBUFFER, minimapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, minimapColourTexture, 0);

	glEnable(GL_CULL_FACE);
	glClearColor(1, 1, 1, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	RenderMinimap();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void NCL::CSC8503::GameTechRenderer::RenderImGUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Debug Window");
	if (ImGui::CollapsingHeader("Camera"))
	{
		if(!followCamera) followCamera = (ToonFollowCamera*)(gameWorld.GetMainCamera());

		Vector3 cPos = gameWorld.GetMainCamera()->GetPosition();
		Vector3 cRot(gameWorld.GetMainCamera()->GetPitch(), gameWorld.GetMainCamera()->GetYaw(), 0);
		Vector3 cFollowOffset = followCamera->GetFollowOffset();
		Vector3 cTargetOffset = followCamera->GetTargetOffset();
		Vector3 cAimOffset = followCamera->GetAimOffset();

		float distance = followCamera->GetFollowDistance();
		float smoothness = followCamera->GetSmoothness();
		float cPitchOffset = followCamera->GetPitchOffset();

		if (ImGui::DragFloat3("Cam Position", (float*)&cPos)) gameWorld.GetMainCamera()->SetPosition(cPos);
		if (ImGui::DragFloat("Cam Pitch", (float*)&cRot.x)) gameWorld.GetMainCamera()->SetPitch(cPos.x);
		if (ImGui::DragFloat("Cam Yaw", (float*)&cRot.y)) gameWorld.GetMainCamera()->SetYaw(cPos.y);
		if (ImGui::DragFloat("Pitch Offset", (float*)&cPitchOffset)) followCamera->SetPitchOffset(cPitchOffset);

		if (ImGui::DragFloat("Follow Distance", (float*)&distance)) followCamera->SetFollowDistance(distance);
		if (ImGui::DragFloat("Follow Smoothness", (float*)&smoothness)) followCamera->SetSmoothness(smoothness);
		if (ImGui::DragFloat3("Follow Offset", (float*)&cFollowOffset)) followCamera->SetFollowOffset(cFollowOffset);
		if (ImGui::DragFloat3("Target Offset", (float*)&cTargetOffset)) followCamera->SetTargetOffset(cTargetOffset);
		if (ImGui::DragFloat3("Aim Offset", (float*)&cAimOffset)) followCamera->SetAimOffset(cAimOffset);
	}
	if (ImGui::CollapsingHeader("Player"))
	{
		Player* player = ToonLevelManager::Get()->GetPlayer();
		Vector3 playerPos = ToonUtils::ConvertToNCLVector3(player->GetRigidbody()->getTransform().getPosition());

		ImGui::DragFloat3("Position", (float*)(&playerPos));
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GameTechRenderer::BuildObjectList() {
	activeObjects.clear();

	gameWorld.OperateOnContents(
		[&](ToonGameObject* o) 
		{
			if (o->IsActive()) 
			{
				o->CalculateModelMatrix();
				activeObjects.emplace_back(o);
				/*const ToonRenderObject* g = o->GetRenderObject();
				if (g) 
				{
					activeObjects.emplace_back(g);
				}*/
			}
		}
	);

	/*gameWorld.OperateOnContents(
		[&](GameObject* o) {
			if (o->IsActive()) {
				const RenderObject* g = o->GetRenderObject();
				if (g) {
					activeObjects.emplace_back(g);
				}
			}
		}
	);*/
}


void GameTechRenderer::PresentScene()
{
	
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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sceneColourTexture);
	glUniform1i(glGetUniformLocation(textureShader->GetProgramID(), "diffuseTex"), 0);
	BindMesh(fullScreenQuad);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	
	DrawMinimapToScreen(modelLocation);

	
	
}

void NCL::CSC8503::GameTechRenderer::DrawMinimapToScreen(int modelLocation)
{
	if (!gameWorld.GetMinimapCamera()) return;
	glEnable(GL_STENCIL_TEST);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_ALWAYS, 2, ~0);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	Matrix4 minimapModelMatrix = Matrix4::Translation(Vector3(-0.8f, -0.7f, 0.0f)) * Matrix4::Scale(Vector3(0.3f, 0.3f, 1.0f));
	glUniformMatrix4fv(modelLocation, 1, false, (float*)&minimapModelMatrix);
	glBindTexture(GL_TEXTURE_2D, minimapColourTexture);
	glUniform1i(glGetUniformLocation(textureShader->GetProgramID(), "diffuseTex"), 0);

	BindMesh(minimapStencilQuad);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 2, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	BindMesh(minimapQuad);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_STENCIL_TEST);
}

void GameTechRenderer::SortObjectList() {

}

void GameTechRenderer::RenderShadowMap() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glCullFace(GL_FRONT);

	BindShader(shadowShader);
	int mvpLocation = glGetUniformLocation(shadowShader->GetProgramID(), "mvpMatrix");

	Matrix4 shadowViewMatrix = Matrix4::BuildViewMatrix(lightPosition, Vector3(0, 0, 0), Vector3(0, 1, 0));
	Matrix4 shadowProjMatrix = Matrix4::Perspective(100.0f, 500.0f, 1, 45.0f);

	Matrix4 mvMatrix = shadowProjMatrix * shadowViewMatrix;

	shadowMatrix = biasMatrix * mvMatrix; //we'll use this one later on

	for (const auto&i : activeObjects) 
	{
		/*Quaternion rot;
		reactphysics3d::Quaternion rRot = (*i).GetRigidbody()->getTransform().getOrientation();
		rot.x = rRot.x;
		rot.y = rRot.y;
		rot.z = rRot.z;
		rot.w = rRot.w;

		Matrix4 modelMatrix = Matrix4::Translation((*i).GetRigidbody()->getTransform().getPosition().x,
			(*i).GetRigidbody()->getTransform().getPosition().y,
			(*i).GetRigidbody()->getTransform().getPosition().z) *

			Matrix4(rot) *

			Matrix4::Scale((*i).GetRenderObject()->GetTransform()->GetScale().x, (*i).GetRenderObject()->GetTransform()->GetScale().y, (*i).GetRenderObject()->GetTransform()->GetScale().z);*/

		Matrix4 modelMatrix = (*i).GetModelMatrix();
		Matrix4 mvpMatrix	= mvMatrix * modelMatrix;
		glUniformMatrix4fv(mvpLocation, 1, false, (float*)&mvpMatrix);
		BindMesh((*i).GetRenderObject()->GetMesh());
		int layerCount = (*i).GetRenderObject()->GetMesh()->GetSubMeshCount();
		for (int i = 0; i < layerCount; ++i) {
			DrawBoundMesh(i);
		}
	}

	glViewport(0, 0, windowWidth, windowHeight);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);

	glCullFace(GL_BACK);
}

void GameTechRenderer::RenderSkybox() {
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	float screenAspect = (float)windowWidth / (float)windowHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);

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

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void GameTechRenderer::RenderCamera() {
	float screenAspect = (float)windowWidth / (float)windowHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);

	OGLShader* activeShader = nullptr;
	int projLocation = 0;
	int viewLocation = 0;
	int modelLocation = 0;
	int colourLocation = 0;
	int hasVColLocation = 0;
	int hasTexLocation = 0;
	int shadowLocation = 0;

	int lightPosLocation = 0;
	int lightColourLocation = 0;
	int lightRadiusLocation = 0;

	int cameraLocation = 0;

	int impactPointsLocation = 0;
	int impactPointCountLocation = 0;

	int objectPosLocation = 0;

	//TODO - PUT IN FUNCTION
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	for (const auto&i : activeObjects) {
		OGLShader* shader = (OGLShader*)(*i).GetRenderObject()->GetShader();
		BindShader(sceneShader);

		BindTextureToShader((OGLTexture*)(*i).GetRenderObject()->GetDefaultTexture(), "mainTex", 0);

		//if (activeShader != shader) {
			projLocation = glGetUniformLocation(sceneShader->GetProgramID(), "projMatrix");
			viewLocation = glGetUniformLocation(sceneShader->GetProgramID(), "viewMatrix");
			modelLocation = glGetUniformLocation(sceneShader->GetProgramID(), "modelMatrix");
			shadowLocation = glGetUniformLocation(sceneShader->GetProgramID(), "shadowMatrix");
			colourLocation = glGetUniformLocation(sceneShader->GetProgramID(), "objectColour");
			hasVColLocation = glGetUniformLocation(sceneShader->GetProgramID(), "hasVertexColours");
			hasTexLocation = glGetUniformLocation(sceneShader->GetProgramID(), "hasTexture");

			lightPosLocation = glGetUniformLocation(sceneShader->GetProgramID(), "lightPos");
			lightColourLocation = glGetUniformLocation(sceneShader->GetProgramID(), "lightColour");
			lightRadiusLocation = glGetUniformLocation(sceneShader->GetProgramID(), "lightRadius");

			cameraLocation = glGetUniformLocation(sceneShader->GetProgramID(), "cameraPos");

			objectPosLocation = glGetUniformLocation(sceneShader->GetProgramID(), "objectPosition");

			glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, 20, "Passing Impact Points");

			impactPointCountLocation = glGetUniformLocation(sceneShader->GetProgramID(), "impactPointCount");

			Vector3 camPos = gameWorld.GetMainCamera()->GetPosition();
			glUniform3fv(cameraLocation, 1, camPos.array);

			Vector3 objPos = ToonUtils::ConvertToNCLVector3((i)->GetRigidbody()->getTransform().getPosition());
			glUniform3fv(objectPosLocation, 1, objPos.array);

			PassImpactPointDetails((*i).GetRenderObject(), impactPointCountLocation, impactPointsLocation, sceneShader);
			glPopDebugGroup();
			

			glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
			glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

			glUniform3fv(lightPosLocation, 1, (float*)&lightPosition);
			glUniform4fv(lightColourLocation, 1, (float*)&lightColour);
			glUniform1f(lightRadiusLocation, lightRadius);

			int shadowTexLocation = glGetUniformLocation(sceneShader->GetProgramID(), "shadowTex");
			glUniform1i(shadowTexLocation, 1);

			activeShader = shader;
		//}

		/*Quaternion rot;
		reactphysics3d::Quaternion rRot = (*i).GetRigidbody()->getTransform().getOrientation();
		rot.x = rRot.x;
		rot.y = rRot.y;
		rot.z = rRot.z;
		rot.w = rRot.w;

		//std::cout << rot << std::endl;

		Matrix4 modelMatrix = Matrix4::Translation((*i).GetRigidbody()->getTransform().getPosition().x,
			(*i).GetRigidbody()->getTransform().getPosition().y,
			(*i).GetRigidbody()->getTransform().getPosition().z) *

			Matrix4(rot) *

			Matrix4::Scale((*i).GetRenderObject()->GetTransform()->GetScale().x, (*i).GetRenderObject()->GetTransform()->GetScale().y, (*i).GetRenderObject()->GetTransform()->GetScale().z);*/

		Matrix4 modelMatrix = (*i).GetModelMatrix();
		glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);			
		
		Matrix4 fullShadowMat = shadowMatrix * modelMatrix;
		glUniformMatrix4fv(shadowLocation, 1, false, (float*)&fullShadowMat);

		Vector4 colour = i->GetRenderObject()->GetColour();
		glUniform4fv(colourLocation, 1, colour.array);

		glUniform1i(hasVColLocation, !(*i).GetRenderObject()->GetMesh()->GetColourData().empty());

		glUniform1i(hasTexLocation, (OGLTexture*)(*i).GetRenderObject()->GetDefaultTexture() ? 1 : 0);

		BindMesh((*i).GetRenderObject()->GetMesh());
		int layerCount = (*i).GetRenderObject()->GetMesh()->GetSubMeshCount();
		for (int i = 0; i < layerCount; ++i) {
			DrawBoundMesh(i);
		}
	}
}

void GameTechRenderer::RenderMinimap()
{
	if (!gameWorld.GetMinimapCamera()) return;
	float screenAspect = (float)windowWidth / (float)windowHeight;
	Matrix4 viewMatrix = gameWorld.GetMinimapCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMinimapCamera()->BuildProjectionMatrix(screenAspect);

	OGLShader* activeShader = minimapShader;

	int projLocation = glGetUniformLocation(activeShader->GetProgramID(), "projMatrix");
	int viewLocation = glGetUniformLocation(activeShader->GetProgramID(), "viewMatrix");
	int modelLocation = glGetUniformLocation(activeShader->GetProgramID(), "modelMatrix");
	int colourLocation = glGetUniformLocation(activeShader->GetProgramID(), "objectColour");
	int hasVColLocation = glGetUniformLocation(activeShader->GetProgramID(), "hasVertexColours");
	int hasTexLocation = glGetUniformLocation(activeShader->GetProgramID(), "hasTexture");
	int objectPosLocation = glGetUniformLocation(activeShader->GetProgramID(), "objectPosition");

	int impactPointsLocation = 0;
	int impactPointCountLocation = glGetUniformLocation(activeShader->GetProgramID(), "impactPointCount");
	BindShader(activeShader);
	for (const auto& i : activeObjects) 
	{
		BindTextureToShader((OGLTexture*)(*i).GetRenderObject()->GetDefaultTexture(), "mainTex", 0);

		PassImpactPointDetails((*i).GetRenderObject(), impactPointCountLocation, impactPointsLocation, activeShader);

			glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
			glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

			Vector3 objPos = ToonUtils::ConvertToNCLVector3((i)->GetRigidbody()->getTransform().getPosition());
			glUniform3fv(objectPosLocation, 1, objPos.array);
		
		Matrix4 modelMatrix = (*i).GetModelMatrix();
		glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

		Vector4 colour = i->GetRenderObject()->GetColour();
		glUniform4fv(colourLocation, 1, colour.array);

		glUniform1i(hasVColLocation, !(*i).GetRenderObject()->GetMesh()->GetColourData().empty());

		glUniform1i(hasTexLocation, (OGLTexture*)(*i).GetRenderObject()->GetDefaultTexture() ? 1 : 0);

		BindMesh((*i).GetRenderObject()->GetMesh());
		int layerCount = (*i).GetRenderObject()->GetMesh()->GetSubMeshCount();
		for (int i = 0; i < layerCount; ++i) {
			DrawBoundMesh(i);
		}
	}
}

void GameTechRenderer::PassImpactPointDetails(const ToonRenderObject* const& i, int impactPointCountLocation, int& impactPointsLocation, OGLShader* shader)
{
	ToonGameObject* linkedObject = (*i).GetGameObject();

	if (dynamic_cast<PaintableObject*>(linkedObject)) {
		PaintableObject* paintedObject = (PaintableObject*)linkedObject;

		std::deque<ImpactPoint>* objImpactPoints = paintedObject->GetImpactPoints(); //change to reference at some point

		glUniform1i(impactPointCountLocation, (int)objImpactPoints->size());

		if (objImpactPoints->empty()) return;

		GLuint i = 0;
		for (const ImpactPoint& point : *objImpactPoints) {
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
}

MeshGeometry* GameTechRenderer::LoadMesh(const string& name) {
	OGLMesh* mesh = new OGLMesh(name);
	mesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	mesh->UploadToGPU();
	return mesh;
}

void GameTechRenderer::NewRenderLines() {
	const std::vector<Debug::DebugLineEntry>& lines = Debug::GetDebugLines();
	if (lines.empty()) {
		return;
	}
	float screenAspect = (float)windowWidth / (float)windowHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);

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



TextureBase* GameTechRenderer::LoadTexture(const string& name) {
	return TextureLoader::LoadAPITexture(name);
}

ShaderBase* GameTechRenderer::LoadShader(const string& vertex, const string& fragment) {
	return new OGLShader(vertex, fragment);
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




void GameTechRenderer::GenerateScreenTexture(GLuint& into, bool depth) {
	glGenTextures(1, &into);
	glBindTexture(GL_TEXTURE_2D, into);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GLuint format = depth ? GL_DEPTH_COMPONENT24 : GL_RGBA8;
	GLuint type = depth ? GL_DEPTH_COMPONENT : GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0,
		format, windowWidth, windowHeight, 0, type, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

}
//void GameTechRenderer::FillBuffers() {
//	float screenAspect = (float)windowWidth / (float)windowHeight;
//	Matrix4 modelMatrix;
//	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
//	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);
//
//	 glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
//	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//	
//	 BindShader(SceneShader);
//	 glUniform1i(
//	 glGetUniformLocation(SceneShader->GetProgramID(), "diffuseTex"), 0);
//	 glUniform1i(
//	 glGetUniformLocation(SceneShader->GetProgramID(), "bumpTex"), 1);
//	
// glActiveTexture(GL_TEXTURE0);
//	 glBindTexture(GL_TEXTURE_2D, earthTex);
//	 glActiveTexture(GL_TEXTURE1);
//	 glBindTexture(GL_TEXTURE_2D, earthBump);
//	 modelMatrix.ToZero();
//	 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
//	 projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
//		 (float)windowWidth / (float)windowHeight, 45.0f);
//	
//	 UpdateShaderMatrices();
//	
//		 heightMap->Draw();
//	
//	 glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	
//}
void GameTechRenderer::DrawPointLights() {
	float screenAspect = (float)windowWidth / (float)windowHeight;
	Matrix4 modelMatrix;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);
	Vector3 CamPos = gameWorld.GetMainCamera()->GetPosition();
	for (const auto& i : activeObjects) {
		glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
		BindShader(PointLightShader);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glBlendFunc(GL_ONE, GL_ONE);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_ALWAYS);
		glDepthMask(GL_FALSE);

		glUniform1i(glGetUniformLocation(
			PointLightShader->GetProgramID(), "depthTex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bufferDepthTex);

		glUniform1i(glGetUniformLocation(
			PointLightShader->GetProgramID(), "normTex"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, bufferNormalTex);

		glUniform3fv(glGetUniformLocation(PointLightShader->GetProgramID(),
			"cameraPos"), 1, (float*)&CamPos);

		glUniform2f(glGetUniformLocation(PointLightShader->GetProgramID(),
			"pixelSize"), 1.0f / windowWidth, 1.0f / windowHeight);

		Matrix4 invViewProj = (projMatrix * viewMatrix).Inverse();
		glUniformMatrix4fv(glGetUniformLocation(
			PointLightShader->GetProgramID(), "inverseProjView"),
			1, false, (float*)&invViewProj);
		UpdateShaderMatrices();
		BindMesh((*i).GetMesh());
		for (int i = 0; i < 24; ++i) {
			Light& l = pointLights[i];
			SetShaderLight(l);
			DrawBoundMesh();

		}

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LEQUAL);

		glDepthMask(GL_TRUE);

		glClearColor(0.2f, 0.2f, 0.2f, 1);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
void GameTechRenderer::CombineBuffers() {
	float screenAspect = (float)windowWidth / (float)windowHeight;
	OGLMesh* quad = new OGLMesh;
	quad = GenQuad();
	BindMesh(quad);
	 BindShader(CombineShader);
	 Matrix4 modelMatrix;
	 Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	 Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);
	
	 modelMatrix.ToZero();
	 viewMatrix.ToZero();
	 projMatrix.ToZero();
	 UpdateShaderMatrices();
	
	 glUniform1i(glGetUniformLocation(
	 CombineShader->GetProgramID(), "diffuseTex"), 0);
	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, bufferColourTex);
	
		 glUniform1i(glGetUniformLocation(
			 CombineShader->GetProgramID(), "diffuseLight"), 1);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, lightDiffuseTex);
	
		 glUniform1i(glGetUniformLocation(
			 CombineShader->GetProgramID(), "specularLight"), 2);
	 glActiveTexture(GL_TEXTURE2);
	 glBindTexture(GL_TEXTURE_2D, lightSpecularTex);
	
	 DrawBoundMesh();
	 
	
}
void GameTechRenderer::UpdateShaderMatrices() {
	OGLShader* boundShader = nullptr;
	for (const auto& i : activeObjects) {
		boundShader = (OGLShader*)(*i).GetShader();
		if (boundShader != nullptr) {
			float screenAspect = (float)windowWidth / (float)windowHeight;
			Matrix4 modelMatrix = (*i).GetTransform()->GetMatrix();
			/*Matrix4 textureMatrix;*/
			Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
			Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);
			int modelLocation = glGetUniformLocation(boundShader->GetProgramID(), "modelMatrix");
			int projLocation = glGetUniformLocation(boundShader->GetProgramID(), "projMatrix");
			int viewLocation = glGetUniformLocation(boundShader->GetProgramID(), "viewMatrix");
			/*int textureLocation = glGetUniformLocation(boundShader->GetProgramID(), "textureMatrix");*/
			glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);
			glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
			glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);
			/*glUniformMatrix4fv(textureLocation, 1, false, (float*)&textureMatrix);*/
			boundShader = nullptr;
		}
	}

}
OGLMesh* GameTechRenderer::GenQuad() {
	 OGLMesh * m = new OGLMesh();
	
	 std::vector<Vector3>vertices;
	 vertices[0] = Vector3(-1.0f, 1.0f, 0.0f);
	 vertices[1] = Vector3(-1.0f, -1.0f, 0.0f);
	 vertices[2] = Vector3(1.0f, 1.0f, 0.0f);
	 vertices[3] = Vector3(1.0f, -1.0f, 0.0f);
	 std::vector<Vector2>textureCoords;
	 m->SetVertexPositions(vertices);
	 
	 textureCoords[0] = Vector2(0.0f, 1.0f);
	 textureCoords[1] = Vector2(0.0f, 0.0f);
	 textureCoords[2] = Vector2(1.0f, 1.0f);
	 textureCoords[3] = Vector2(1.0f, 0.0f);
	 m->SetVertexTextureCoords(textureCoords);
	 std::vector<Vector4>colours;
	 for (int i = 0; i < 4; ++i) {
		 colours[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	 }
	 m->SetVertexColours(colours);
//m->BufferData();
	 m->UploadToGPU();
 return m;
	
}
void GameTechRenderer::SetShaderLight(  const Light &l) {
	
	OGLShader* boundShader = nullptr;
	Vector3 position = l.GetPosition();
	Vector4 colour = l.GetColour();
	float radius = l.GetRadius();
	
	boundShader = PointLightShader;
	glUniform3fv(glGetUniformLocation(boundShader->GetProgramID(),
		"lightPos"), 1, (float*)&position);

	glUniform4fv(glGetUniformLocation(boundShader->GetProgramID(),
		"lightColour"), 1, (float*)&colour);

	glUniform1f(glGetUniformLocation(boundShader->GetProgramID(),
		"lightRadius"), radius);

}

