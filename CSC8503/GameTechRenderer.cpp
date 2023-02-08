#include "GameTechRenderer.h"
#include "GameObject.h"
#include "ToonGameObject.h"
#include "RenderObject.h"
#include "ToonRenderObject.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "ImpactPoint.h"
#include "PaintableObject.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

#define SHADOWSIZE 4096

Matrix4 biasMatrix = Matrix4::Translation(Vector3(0.5f, 0.5f, 0.5f)) * Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f));


GameTechRenderer::GameTechRenderer(ToonGameWorld& world) : OGLRenderer(*Window::GetWindow()), gameWorld(world)	{
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
	shadowShader = new OGLShader("shadow.vert", "shadow.frag");
	minimapShader = new OGLShader("minimap.vert", "minimap.frag");
	textureShader = new OGLShader("Texture.vert", "Texture.frag");

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
	minimapStencilQuad->SetVertexPositions({ Vector3(-0.5, 0.8,-1), Vector3(-0.5,-0.8,-1) , Vector3(0.5,-0.8,-1) , Vector3(0.5,0.8,-1) });
	minimapStencilQuad->SetVertexIndices({ 0,1,2,2,3,0 });
	minimapStencilQuad->UploadToGPU();
	

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
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, 3, "123");
	PresentScene();
	glPopDebugGroup();
	

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
	RenderCamera();
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

void GameTechRenderer::BuildObjectList() {
	activeObjects.clear();

	gameWorld.OperateOnContents(
		[&](ToonGameObject* o) 
		{
			if (o->IsActive()) 
			{
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
	glEnable(GL_STENCIL_TEST);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_ALWAYS, 2, ~0);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	Matrix4 minimapModelMatrix = Matrix4::Translation(Vector3(-0.8, -0.7, 0)) * Matrix4::Scale(Vector3(0.3f, 0.3f, 1));
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
		Quaternion rot;
		reactphysics3d::Quaternion rRot = (*i).GetRigidbody()->getTransform().getOrientation();
		rot.x = rRot.x;
		rot.y = rRot.y;
		rot.z = rRot.z;
		rot.w = rRot.w;

		Matrix4 modelMatrix = Matrix4::Translation((*i).GetRigidbody()->getTransform().getPosition().x,
			(*i).GetRigidbody()->getTransform().getPosition().y,
			(*i).GetRigidbody()->getTransform().getPosition().z) *

			Matrix4(rot) *

			Matrix4::Scale((*i).GetRenderObject()->GetTransform()->GetScale().x, (*i).GetRenderObject()->GetTransform()->GetScale().y, (*i).GetRenderObject()->GetTransform()->GetScale().z);

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

	//TODO - PUT IN FUNCTION
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	for (const auto&i : activeObjects) {
		OGLShader* shader = (OGLShader*)(*i).GetRenderObject()->GetShader();
		BindShader(shader);

		BindTextureToShader((OGLTexture*)(*i).GetRenderObject()->GetDefaultTexture(), "mainTex", 0);

		if (activeShader != shader) {
			projLocation = glGetUniformLocation(shader->GetProgramID(), "projMatrix");
			viewLocation = glGetUniformLocation(shader->GetProgramID(), "viewMatrix");
			modelLocation = glGetUniformLocation(shader->GetProgramID(), "modelMatrix");
			shadowLocation = glGetUniformLocation(shader->GetProgramID(), "shadowMatrix");
			colourLocation = glGetUniformLocation(shader->GetProgramID(), "objectColour");
			hasVColLocation = glGetUniformLocation(shader->GetProgramID(), "hasVertexColours");
			hasTexLocation = glGetUniformLocation(shader->GetProgramID(), "hasTexture");

			lightPosLocation = glGetUniformLocation(shader->GetProgramID(), "lightPos");
			lightColourLocation = glGetUniformLocation(shader->GetProgramID(), "lightColour");
			lightRadiusLocation = glGetUniformLocation(shader->GetProgramID(), "lightRadius");

			cameraLocation = glGetUniformLocation(shader->GetProgramID(), "cameraPos");


			impactPointCountLocation = glGetUniformLocation(shader->GetProgramID(), "impactPointCount");

			Vector3 camPos = gameWorld.GetMainCamera()->GetPosition();
			glUniform3fv(cameraLocation, 1, camPos.array);

			PassImpactPointDetails((*i).GetRenderObject(), impactPointCountLocation, impactPointsLocation, shader);

			glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
			glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

			glUniform3fv(lightPosLocation, 1, (float*)&lightPosition);
			glUniform4fv(lightColourLocation, 1, (float*)&lightColour);
			glUniform1f(lightRadiusLocation, lightRadius);

			int shadowTexLocation = glGetUniformLocation(shader->GetProgramID(), "shadowTex");
			glUniform1i(shadowTexLocation, 1);

			activeShader = shader;
		}

		Quaternion rot;
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

			Matrix4::Scale((*i).GetRenderObject()->GetTransform()->GetScale().x, (*i).GetRenderObject()->GetTransform()->GetScale().y, (*i).GetRenderObject()->GetTransform()->GetScale().z);

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

	int impactPointsLocation = 0;
	int impactPointCountLocation = glGetUniformLocation(activeShader->GetProgramID(), "impactPointCount");
	BindShader(activeShader);
	for (const auto& i : activeObjects) {
			

			BindTextureToShader((OGLTexture*)(*i).GetRenderObject()->GetDefaultTexture(), "mainTex", 0);

			PassImpactPointDetails((*i).GetRenderObject(), impactPointCountLocation, impactPointsLocation, activeShader);

			glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
			glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);
		

			Quaternion rot;
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

				Matrix4::Scale((*i).GetRenderObject()->GetTransform()->GetScale().x, (*i).GetRenderObject()->GetTransform()->GetScale().y, (*i).GetRenderObject()->GetTransform()->GetScale().z);

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

		std::deque objImpactPoints = paintedObject->GetImpactPoints(); //change to reference at some point

		glUniform1i(impactPointCountLocation, objImpactPoints.size());

		GLuint i = 0;
		for (const ImpactPoint& point : objImpactPoints) {
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

	int frameLineCount = lines.size() * 2;

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


