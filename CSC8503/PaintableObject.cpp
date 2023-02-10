#include "PaintableObject.h"
#include "ToonUtils.h"
#include "ToonGameWorld.h"

using namespace NCL::CSC8503;

static int i = 0;

PaintableObject::PaintableObject(reactphysics3d::PhysicsWorld& RP3D_World) : ToonGameObject(RP3D_World) {
}

void PaintableObject::Update(float dt) {

}


void PaintableObject::AddImpactPoint(ImpactPoint point) { //Impact point with world space cooridnates passed in; convert to local space.
	int impactCount = (int)impactPoints.size();

	if (impactCount < 10) {
		impactPoints.push_back(point);
	}
	else {
		impactPoints.pop_front();
		impactPoints.push_back(point);
		
	}
	
}