#include "ToonGameObjectAnim.h"
#include "OGLMesh.h"
#include "OGLShader.h"
#include "ToonAssetManager.h"
#include "Debug.h"

int index = 0;
NCL::CSC8503::ToonGameObjectAnim::ToonGameObjectAnim(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld) : ToonGameObject(RP3D_World, gameWorld)
{
	hasSkin = true;
	currentAnimSpeed = 1.0f;

	tweenBlendFactor = 0.0f;
	tweenTime = 0.0f;
	tweenTimeCurrent = 0.0f;
	isTweening = false;

	pendingAnim = nullptr;
}

NCL::CSC8503::ToonGameObjectAnim::~ToonGameObjectAnim()
{
	
}

void NCL::CSC8503::ToonGameObjectAnim::Update(float dt)
{
	if (!currentAnim) return;

	if (isTweening && pendingAnim != nullptr)
	{
		tweenBlendFactor += dt * (1.0f / tweenTime);
		tweenTimeCurrent -= dt;
		if (tweenTimeCurrent <= 0.0f)
		{
			tweenTimeCurrent = 0.0f;
			isTweening = false;
			currentFrame = 0;
			currentAnim = pendingAnim;
		}
	}
	else
	{
		frameTime -= dt * currentAnimSpeed;
		while (frameTime < 0.0f) 
		{
			currentFrame = (currentFrame + 1) % currentAnim->GetFrameCount();
			nextFrame = (currentFrame + 1) % currentAnim->GetFrameCount();
			frameTime += 1.0f / currentAnim->GetFrameRate();
		}
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
		/*int headIndex = mesh->GetIndexForJoint("mixamorig:Head");
		Matrix4 headJoint = frameData[headIndex];

		int eyeLeftIndex = mesh->GetIndexForJoint("mixamorig:LeftEye");
		Matrix4 eyeLeftJoint = frameData[eyeLeftIndex];

		int eyeRightIndex = mesh->GetIndexForJoint("mixamorig:RightEye");
		Matrix4 eyeRightJoint = frameData[eyeRightIndex];

		headJoint = headJoint * Matrix4::Rotation(std::sinf(Window::GetTimer()->GetTotalTimeSeconds()) * 30.0f, Vector3(0, 1, 0)) * Matrix4::Scale(Vector3(2.0f, 2.0f, 2.0f))*/;
		//eyeLeftJoint.SetPositionVector(headJoint.GetPositionVector());
		//eyeRightJoint.SetPositionVector(headJoint.GetPositionVector());

		/*for (unsigned int i = 0; i < mesh->GetJointCount(); i++)
		{
			if(i == headIndex)
				frameMatrices.emplace_back(headJoint * invBindPose[i]);
			else if(i == eyeLeftIndex)
				frameMatrices.emplace_back(headJoint * eyeLeftJoint * invBindPose[i]);
			else if(i == eyeRightIndex)
				frameMatrices.emplace_back(headJoint * eyeRightJoint * invBindPose[i]);
			else
				frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
		}*/

		const Matrix4* invBindPose = mesh->GetInverseBindPose().data();
		if (isTweening && (pendingAnim != nullptr))
		{
			const Matrix4* animCurrentFrame = currentAnim->GetJointData(currentFrame);
			const Matrix4* animPendingFrame = pendingAnim->GetJointData(0);

			std::vector<Matrix4> finalBlending;
			for (size_t i = 0; i < mesh->GetJointCount(); i++)
				finalBlending.emplace_back(ToonUtils::LerpMat(animCurrentFrame[i], animPendingFrame[i], tweenBlendFactor));

			for (size_t i = 0; i < mesh->GetJointCount(); i++)
				frameMatrices.emplace_back(finalBlending[i] * invBindPose[i]);
		}
		else
		{
			const Matrix4* frameData = currentAnim->GetJointData(currentFrame);

			for (unsigned int i = 0; i < mesh->GetJointCount(); i++)
				frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
		}

		int j = glGetUniformLocation(shader->GetProgramID(), "joints");
		glUniformMatrix4fv(j, frameMatrices.size(), false, (float*)frameMatrices.data());

		frameMatrices.clear();
	}

	r.BindMesh(mesh);
	for (int i = 0; i < (int)mesh->GetSubMeshCount(); i++)
	{
		if (renderObject->GetMaterial() != nullptr)
		{
			if (renderObject->GetMaterial() != nullptr && (int)renderObject->GetMaterial()->GetSubMaterials().size() > 0) {
				int materialIndexLocations = glGetUniformLocation(r.GetBoundShader()->GetProgramID(), "materialIndex");
				int index = renderObject->GetMaterial()->GetMaterialIndex()[i];
				glUniform1i(materialIndexLocations, index);
			}

		}

		r.DrawBoundMesh(i);
	}
}

void NCL::CSC8503::ToonGameObjectAnim::PlayAnim(const std::string& anim, bool tween, float animSpeed)
{
	if (anim.empty() || currentAnim == meshAnims.at(anim) || meshAnims.at(anim) == nullptr)
		return;

	if(currentAnim == nullptr)
		currentAnim = meshAnims[anim];

	if (tween)
	{
		pendingAnim = meshAnims[anim];
		TweenAnim(0.15f);
	}
	else
	{
		currentFrame = 0;
		currentAnimSpeed = animSpeed;
		currentAnim = meshAnims[anim];
	}
}

void NCL::CSC8503::ToonGameObjectAnim::TweenAnim(const float& time)
{
	if (isTweening) return;

	isTweening = true;
	tweenTime = time;
	tweenTimeCurrent = time;
	tweenBlendFactor = 0.0f;
}

bool NCL::CSC8503::ToonGameObjectAnim::LoadAnim(const std::string& animationName)
{
	meshAnims[animationName] = ToonAssetManager::Instance().GetAnimation(animationName);
	return(meshAnims[animationName] != nullptr);
}
