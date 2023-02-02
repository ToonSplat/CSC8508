#include "PaintableObject.h"

using namespace NCL::CSC8503;

PaintableObject::PaintableObject(void) {
	owner = nullptr;
	parent = nullptr;
}

PaintableObject::PaintableObject(PaintableZone* parent) : parent(parent) {
	owner = nullptr;
	parent->AddObject();
}

PaintableObject::PaintableObject(PaintableZone* parent, Team* owner) : parent(parent), owner(owner) {
	parent->AddObject(owner);
}

void PaintableObject::Update(float dt) {

}

void PaintableObject::Hit(Team* hitBy) {
	if (parent)
		parent->ChangeChildOwner(owner, hitBy);
	owner = hitBy;
}

void PaintableObject::AddImpactPoint(ImpactPoint point) {
	int impactCount = impactPoints.size();

	if (impactCount < 10) {
		impactPoints.push_back(point);
	}
	else {
		impactPoints.pop_front();
		impactPoints.push_back(point);
	}


}