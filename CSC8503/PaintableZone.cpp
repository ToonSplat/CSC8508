#include "PaintableZone.h"

using namespace NCL::CSC8503;

PaintableZone::PaintableZone(void) {
	owner = nullptr;
	unownedObjects = 0;
}

void PaintableZone::AddObject(void) {
	unownedObjects++;
}

void PaintableZone::ChangeChildOwner(Team* oldOwner, Team* newOwner) {
	const int OBJECT_VALUE = 1;
	if (!oldOwner)
		unownedObjects -= OBJECT_VALUE;
	else if (teamObjectsOwned.find(oldOwner) != teamObjectsOwned.end())
		teamObjectsOwned[oldOwner] -= OBJECT_VALUE;
	else{
		std::cout << "ERROR: ATTEMPTING TO REMOVE OBJECT FROM UNIDENTIFIED OWNER\n";
		return;
	}
	if (!oldOwner)
		unownedObjects += OBJECT_VALUE;
	else if (teamObjectsOwned.find(oldOwner) != teamObjectsOwned.end())
		teamObjectsOwned[oldOwner] += OBJECT_VALUE;
	else teamObjectsOwned.emplace(newOwner, OBJECT_VALUE);

	CheckZoneOwner();
}

void PaintableZone::CheckZoneOwner(void) {
	Team* currentOwner = nullptr;
	int ownerScore = unownedObjects;
	for (auto& team : teamObjectsOwned)
		if (team.second > ownerScore) {
			currentOwner = team.first;
			ownerScore = team.second;
		}
	owner = currentOwner;
}