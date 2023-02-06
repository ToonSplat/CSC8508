#pragma once
#include "ToonGameObject.h"
#include "Team.h"

namespace NCL 
{
	namespace CSC8503
	{
		class PaintBallProjectile : public ToonGameObject 
		{
		public:
			PaintBallProjectile(reactphysics3d::PhysicsWorld& RP3D_World, const reactphysics3d::Vector3& position,
				const reactphysics3d::Vector3& rotationEuler, const float& radius, const float& _impactSize, Team* _team, float alpha = 1.0f);
			~PaintBallProjectile();

			float GetImpactSize() const { return impactSize; }
			Team* GetTeam() const { return team; }

		protected:
			Team* team;
			float impactSize;
		};
	}
}