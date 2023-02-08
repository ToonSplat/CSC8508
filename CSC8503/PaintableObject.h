#pragma once
#include "ToonGameObject.h"
#include "ImpactPoint.h"

namespace NCL {
	namespace CSC8503 {
		class PaintableObject : public ToonGameObject {
		public:
			PaintableObject(reactphysics3d::PhysicsWorld& RP3D_World);
			~PaintableObject(void) = default;

			void Update(float dt) override;

			void AddImpactPoint(ImpactPoint point);

			std::deque<ImpactPoint>* GetImpactPoints() {
				return &impactPoints;
			}

		private:
			std::deque<ImpactPoint> impactPoints;
		};
	}
}