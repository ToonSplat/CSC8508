#pragma once
#include "ToonGameObject.h"

namespace NCL 
{
	namespace CSC8503
	{
		class PaintBallProjectile : public ToonGameObject 
		{
		public:
			PaintBallProjectile(reactphysics3d::PhysicsWorld& RP3D_World, const reactphysics3d::Vector3& position, const reactphysics3d::Vector3& rotationEuler, const float& radius, const float& impactSize);
			~PaintBallProjectile();

			float GetImpactSize() const { return impactSize; }

		protected:
			float impactSize;
		};
	}
}