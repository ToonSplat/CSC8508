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
		virtual void Draw(OGLRenderer& r) override;

		void SetMeshMaterial(MeshMaterial* newMaterial) { meshMaterial = newMaterial; }
		void PlayAnim(const std::string& anim);

	protected:
		bool LoadAnim(MeshAnimation** anim, const std::string& animFile);

	protected:
		MeshMaterial* meshMaterial;

		int		currentFrame;
		int		nextFrame;
		float	frameTime;

		int currentAnimIndex;
		MeshAnimation* currentAnim;
		std::map<std::string, MeshAnimation*> meshAnims;
	};
}