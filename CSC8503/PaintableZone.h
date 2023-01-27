#pragma once
#include "Team.h"
#include <unordered_map>

namespace NCL {
	namespace CSC8503 {
		class PaintableZone {
		public:
			PaintableZone(void);
			~PaintableZone(void) = default;

			void AddObject(void);
			void ChangeChildOwner(Team* oldOwner, Team* newOwner);

			Team* GetOwner(void) const { return owner; }

		private:
			void CheckZoneOwner(void);
			Team* owner;
			unordered_map<Team*, int> teamObjectsOwned;
			int unownedObjects;
		};
	}
}