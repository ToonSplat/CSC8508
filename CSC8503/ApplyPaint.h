#pragma once
#include "ImpactPoint.h"
#include <deque>
namespace NCL {
	namespace CSC8503 {
		class ApplyPaint {
		public:
			ApplyPaint() {
				isFloor = false;
			};
			~ApplyPaint() {};
			void AddImpactPoint(ImpactPoint point) {
				int impactCount = (int)impactPoints.size();

				if (impactCount < 300) { // change max value from hardcoded
					impactPoints.push_back(point);
				}
				else {
					impactPoints.pop_front();
					impactPoints.push_back(point);

				}
			}

			std::deque<ImpactPoint>* GetImpactPoints() {
				return &impactPoints;
			}

			void SetAsFloor() { isFloor = true; }
			bool IsObjectTheFloor() { return isFloor; }

		protected:
			std::deque<ImpactPoint> impactPoints;
			bool isFloor;
		};
	}
}