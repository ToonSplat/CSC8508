#include "PaintableObject.h"
#include "ToonUtils.h"
#include "ToonGameWorld.h"

using namespace NCL::CSC8503;

PaintableObject::PaintableObject(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld) : ToonGameObject(RP3D_World, gameWorld) {
}

void PaintableObject::Update(float dt) {

}


void PaintableObject::AddImpactPoint(ImpactPoint point) { //Impact point with world space cooridnates passed in; convert to local space.
	int impactCount = (int)impactPoints.size();

	if (impactCount < 300) { // change max value from hardcoded
		impactPoints.push_back(point);
	}
	else {
		impactPoints.pop_front();
		impactPoints.push_back(point);
		
	}
	
}