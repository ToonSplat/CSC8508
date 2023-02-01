#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "Vector3.h"
#include "ToonUtils.h"

using namespace reactphysics3d;

namespace NCL
{
	namespace CSC8503
	{
		class ToonRaycastCallback : public RaycastCallback
		{
		public:
			virtual float notifyRaycastHit(const RaycastInfo& info)
			{
				raycastHitWorldPos = ToonUtils::ConvertToNCLVector3(info.worldPoint);
				raycastHitNormal = ToonUtils::ConvertToNCLVector3(info.worldNormal);

				raycastTriangleIndex = info.triangleIndex;

				raycastCollisionBody = info.body;
				raycastCollider = info.collider;

				return decimal(0.0f);
			}

			NCL::Maths::Vector3 GetHitWorldPos() const { return raycastHitWorldPos; }
			NCL::Maths::Vector3 GetHitNormal() const { return raycastHitNormal; }
			int GetTriangleIndex() const { return raycastTriangleIndex; }
			CollisionBody* GetHitBody() const { return raycastCollisionBody; }
			Collider* GetHitCollider() const { return raycastCollider; }

		private:
			NCL::Maths::Vector3 raycastHitWorldPos;
			NCL::Maths::Vector3 raycastHitNormal;
			int raycastTriangleIndex;
			CollisionBody* raycastCollisionBody;
			Collider* raycastCollider;
		};
	}
}