#pragma once
#include "Team.h"
#include <unordered_map>

namespace NCL {
	namespace CSC8503 {
		class PaintableZone {
		public:
			PaintableZone(void);
			PaintableZone(PaintableZone* parent);
			~PaintableZone(void) = default;

			void AddObject(void);
			void AddObject(Team* owner);
			void RemoveObject(Team* owner);

			void ChangeChildOwner(Team* oldOwner, Team* newOwner);

			Team* GetOwner(void) { if (owner) return owner; else return nullptr; }

			int GetTotalObjects(void);

			void PrintOwnership(void);

		private:
			void CheckZoneOwner(void);
			Team* owner;
			PaintableZone* parent;
			unordered_map<Team*, int> teamObjectsOwned;
			int unownedObjects;
		};
	}
}