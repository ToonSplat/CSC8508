#pragma once
#include "ToonGameObject.h"
#include "ImpactPoint.h"

namespace NCL {
	namespace CSC8503 {
		class PaintableObject : public ToonGameObject {
		public:
			PaintableObject(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld);
			~PaintableObject(void) = default;

			//void Update(float dt) override;

			void AddImpactPoint(ImpactPoint point);

			std::deque<ImpactPoint>* GetImpactPoints() {
				return &impactPoints;
			}

			void SetAsFloor() { isFloor = true; }
			bool IsObjectTheFloor() { return isFloor; }

		private:
			std::deque<ImpactPoint> impactPoints;
			bool isFloor;
		};
	}
}