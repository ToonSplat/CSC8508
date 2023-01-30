#include "PaintableZone.h"
#include <iostream>

using namespace NCL::CSC8503;

PaintableZone::PaintableZone(void) {
	owner = nullptr;
	unownedObjects = 0;
	teamObjectsOwned.clear();
}

void PaintableZone::AddObject(void) {
	unownedObjects++;
}

void PaintableZone::AddObject(Team* owner) {
	const int OBJECT_VALUE = 1;
	if (!owner)
		unownedObjects += OBJECT_VALUE;
	else if (teamObjectsOwned.find(owner) != teamObjectsOwned.end())
		teamObjectsOwned[owner] += OBJECT_VALUE;
	else teamObjectsOwned.emplace(owner, OBJECT_VALUE);
}

void PaintableZone::RemoveObject(Team* owner) {
	const int OBJECT_VALUE = 1;
	if (!owner)
		unownedObjects -= OBJECT_VALUE;
	else if (teamObjectsOwned.find(owner) != teamObjectsOwned.end())
		teamObjectsOwned[owner] -= OBJECT_VALUE;
	else {
		std::cout << "ERROR: ATTEMPTING TO REMOVE OBJECT FROM UNIDENTIFIED OWNER\n";
	}
}

void PaintableZone::ChangeChildOwner(Team* oldOwner, Team* newOwner) {
	if (oldOwner == newOwner)
		return;
	RemoveObject(oldOwner);
	AddObject(newOwner);

	if (newOwner != owner)
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
	// TODO: Resolve Ties better
}