#include "ToonGameObjectAnim.h"
#include "OGLMesh.h"
#include "OGLShader.h"
#include "ToonAssetManager.h"

int index = 0;
NCL::CSC8503::ToonGameObjectAnim::ToonGameObjectAnim(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld) : ToonGameObject(RP3D_World, gameWorld)
{
	hasSkin = true;
	currentAnimSpeed = 1.0f;
}

NCL::CSC8503::ToonGameObjectAnim::~ToonGameObjectAnim()
{
	
}

void NCL::CSC8503::ToonGameObjectAnim::Update(float dt)
{
	if (!currentAnim) return;

	frameTime -= dt * currentAnimSpeed;
	while (frameTime < 0.0f) 
	{
		currentFrame = (currentFrame + 1) % currentAnim->GetFrameCount();
		nextFrame = (currentFrame + 1) % currentAnim->GetFrameCount();
		frameTime += 1.0f / currentAnim->GetFrameRate();
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::L))
	{
		index++;
		index %= renderObject->GetMesh()->GetSubMeshCount();
		std::cout << index << std::endl;
	}
}

void NCL::CSC8503::ToonGameObjectAnim::Draw(OGLRenderer& r, bool isMinimap)
{
	if (!renderObject || !renderObject->GetMesh())
		return;

	if (isMinimap)
	{
		OGLMesh* minimapMesh = (OGLMesh*)renderObject->GetMinimapMesh();
		r.BindMesh(minimapMesh);

		for (int i = 0; i < (int)minimapMesh->GetSubMeshCount(); ++i)
			r.DrawBoundMesh(i);

		return;
	}

	OGLMesh* mesh = (OGLMesh*)renderObject->GetMesh();
	OGLShader* shader = r.GetBoundShader();
	if (shader == (OGLShader*)renderObject->GetShader()) {

		const Matrix4* invBindPose = mesh->GetInverseBindPose().data();
		const Matrix4* frameData = currentAnim->GetJointData(currentFrame);

		for (unsigned int i = 0; i < mesh->GetJointCount(); i++)
			frameMatrices.emplace_back(frameData[i] * invBindPose[i]);

		int j = glGetUniformLocation(shader->GetProgramID(), "joints");
		glUniformMatrix4fv(j, frameMatrices.size(), false, (float*)frameMatrices.data());

		frameMatrices.clear();
	}

	r.BindMesh(mesh);
	for (int i = 0; i < (int)mesh->GetSubMeshCount(); i++)
	{
		if (renderObject->GetMaterial() != nullptr)
		{
			if ((int)renderObject->GetMaterial()->GetDiffuseTextures().size() > 0)
				r.BindTextureToShader((NCL::Rendering::OGLTexture*)renderObject->GetMaterial()->GetDiffuseTextures()[i], "mainTex", 0);
		}

		r.DrawBoundMesh(i);
	}
}

void NCL::CSC8503::ToonGameObjectAnim::PlayAnim(const std::string& anim, float animSpeed)
{
	if (anim.empty() || currentAnim == meshAnims.at(anim) || meshAnims.at(anim) == nullptr)
		return;
	currentFrame = 0;
	currentAnimSpeed = animSpeed;
	currentAnim = meshAnims[anim];
}

bool NCL::CSC8503::ToonGameObjectAnim::LoadAnim(const std::string& animationName)
{
	meshAnims[animationName] = ToonAssetManager::Instance().GetAnimation(animationName);
	return(meshAnims[animationName] != nullptr);
}
