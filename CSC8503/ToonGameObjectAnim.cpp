#include "ToonGameObjectAnim.h"
#include "OGLMesh.h"
#include "OGLShader.h"


NCL::CSC8503::ToonGameObjectAnim::ToonGameObjectAnim(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld) : ToonGameObject(RP3D_World, gameWorld)
{
}

NCL::CSC8503::ToonGameObjectAnim::~ToonGameObjectAnim()
{
	for (auto& anim : meshAnims)
		delete anim.second;
}

void NCL::CSC8503::ToonGameObjectAnim::Update(float dt)
{
	if (!currentAnim) return;

	frameTime -= dt;
	while (frameTime < 0.0f) 
	{
		currentFrame = (currentFrame + 1) % currentAnim->GetFrameCount();
		nextFrame = (currentFrame + 1) % currentAnim->GetFrameCount();
		frameTime += 1.0f / currentAnim->GetFrameRate();
	}
}

void NCL::CSC8503::ToonGameObjectAnim::Draw(OGLRenderer& r)
{
	if (!renderObject || !renderObject->GetMesh())
		return;

	OGLMesh* mesh = (OGLMesh*)renderObject->GetMesh();
	OGLShader* shader = (OGLShader*)renderObject->GetShader();

	std::vector<Matrix4> frameMatrices;
	const Matrix4* invBindPose = mesh->GetInverseBindPose().data();
	const Matrix4* frameData = currentAnim->GetJointData(currentFrame);

	for (unsigned int i = 0; i < mesh->GetJointCount(); ++i)
	{
		frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
	}

	int j = glGetUniformLocation(shader->GetProgramID(), "joints");
	glUniformMatrix4fv(j, (GLsizei)frameMatrices.size(), false, (float*)frameMatrices.data());

	r.BindMesh(mesh);
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i)
	{
		//To Add Textures
		r.DrawBoundMesh(i);
	}
}

void NCL::CSC8503::ToonGameObjectAnim::PlayAnim(const std::string& anim)
{
	if (anim.empty() || currentAnim == meshAnims.at(anim) || meshAnims.at(anim) == nullptr)
		return;

	currentFrame = 0;
	currentAnim = meshAnims[anim];
}

bool NCL::CSC8503::ToonGameObjectAnim::LoadAnim(MeshAnimation** anim, const std::string& animFile)
{
	*anim = new MeshAnimation(animFile);
	if (anim == nullptr)
		return false;

	return true;
}
