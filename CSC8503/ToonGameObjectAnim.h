#pragma once
#include "ToonGameObject.h"
#include "MeshAnimation.h"
#include "MeshMaterial.h"

namespace NCL::CSC8503
{
	class ToonGameObjectAnim : public ToonGameObject
	{
	public:
		ToonGameObjectAnim(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld);
		~ToonGameObjectAnim();

		virtual void Update(float dt) override;
		virtual void Draw(OGLRenderer& r, bool isMinimap = false) override;

		void SetMeshMaterial(MeshMaterial* newMaterial) { meshMaterial = newMaterial; }
		void PlayAnim(const std::string& anim, float animSpeed = 1.0f);

		const std::vector<Matrix4> GetFrameMat() const { return frameMatrices; }

	protected:
		bool LoadAnim(const std::string& animationName);

	protected:
		MeshMaterial* meshMaterial;

		int		currentFrame;
		int		nextFrame;
		float	frameTime;

		float	currentAnimSpeed;
		MeshAnimation* currentAnim;
		std::vector<Matrix4> frameMatrices;
		std::map<std::string, MeshAnimation*> meshAnims;
	};
}