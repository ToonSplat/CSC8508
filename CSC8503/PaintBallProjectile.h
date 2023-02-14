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
			PaintBallProjectile(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld, const float& _impactSize, Team* _team);
			~PaintBallProjectile();

			float GetImpactSize() const { return impactSize; }
			Team* GetTeam() const { return team; }

		protected:
			Team* team;
			float impactSize;
		};
	}
}