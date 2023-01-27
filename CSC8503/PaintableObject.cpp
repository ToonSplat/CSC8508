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

void PaintableObject::Hit(Team* hitBy) {
	if (parent)
		parent->ChangeChildOwner(owner, hitBy);
	owner = hitBy;
}