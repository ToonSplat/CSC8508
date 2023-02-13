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
			ToonRaycastCallback()
			{
				isHit = false;
				raycastHitWorldPos = Vector3();
				raycastHitNormal = Vector3();
				raycastTriangleIndex = -1;

				raycastCollisionBody = nullptr;
				raycastCollider = nullptr;

				raycastHitFraction = 0.0f;
			}

			virtual float notifyRaycastHit(const RaycastInfo& info)
			{
				raycastHitWorldPos = ToonUtils::ConvertToNCLVector3(info.worldPoint);
				raycastHitNormal = ToonUtils::ConvertToNCLVector3(info.worldNormal);

				raycastTriangleIndex = info.triangleIndex;

				raycastCollisionBody = info.body;
				raycastCollider = info.collider;
				raycastHitFraction = info.hitFraction;

				isHit = true;

				return decimal(0.0f);
			}

			NCL::Maths::Vector3 GetHitWorldPos() const { return raycastHitWorldPos; }
			NCL::Maths::Vector3 GetHitNormal() const { return raycastHitNormal; }
			int GetTriangleIndex() const { return raycastTriangleIndex; }
			float GetHitFraction() const { return raycastHitFraction; }
			CollisionBody* GetHitBody() const { return raycastCollisionBody; }
			Collider* GetHitCollider() const { return raycastCollider; }
			bool IsHit() const { return isHit; }


		private:			
			NCL::Maths::Vector3 raycastHitWorldPos;
			NCL::Maths::Vector3 raycastHitNormal;
			int raycastTriangleIndex;
			CollisionBody* raycastCollisionBody;
			Collider* raycastCollider;
			float raycastHitFraction;
			bool isHit;
		};
	}
}