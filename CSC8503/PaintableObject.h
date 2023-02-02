#pragma once
#include "PaintableZone.h"
#include "GameObject.h"
#include "ImpactPoint.h"

namespace NCL {
	namespace CSC8503 {
		class PaintableObject : public GameObject {
		public:
			PaintableObject(void);
			PaintableObject(PaintableZone* parent);
			PaintableObject(PaintableZone* parent, Team* owner);
			~PaintableObject(void) = default;

			void Update(float dt);

			void Hit(Team* hitBy);

			void AddImpactPoint(ImpactPoint point);

			std::deque<ImpactPoint> GetImpactPoints() const {
				return impactPoints;
			}

		private:
			Team* owner;
			PaintableZone* parent;
			const int OBJECT_SCORE = 1;
			std::deque<ImpactPoint> impactPoints;
		};
	}
}