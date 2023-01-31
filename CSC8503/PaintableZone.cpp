#include "PaintableZone.h"
#include <iostream>

using namespace NCL::CSC8503;

PaintableZone::PaintableZone(void) {
	owner = nullptr;
	parent = nullptr;
	unownedObjects = 0;
	teamObjectsOwned.clear();
}

PaintableZone::PaintableZone(PaintableZone* parent) : parent(parent) {
	owner = nullptr;
	unownedObjects = 0;
	teamObjectsOwned.clear();
	parent->AddObject();
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
	if (currentOwner != owner) {
		if (parent)
			parent->ChangeChildOwner(owner, currentOwner);
		owner = currentOwner;
	}
	// TODO: Resolve Ties better
}

int PaintableZone::GetTotalObjects(void) {
	int count = unownedObjects;
	for (auto& team : teamObjectsOwned)
		count += team.second;
	return count;
}

void PaintableZone::PrintOwnership(void) {
	std::cout << "Ownership of Zone " << this << std::endl;
	std::cout << "No Team: " << unownedObjects << std::endl;
	for (auto& team : teamObjectsOwned)
		std::cout << team.first->getTeamName() << ": " << team.second << std::endl;
}