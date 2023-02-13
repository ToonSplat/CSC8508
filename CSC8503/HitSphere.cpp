#include "HitSphere.h"
#include "PaintableObject.h"
#include "ToonGameWorld.h"
#include "ToonUtils.h"
#include "ToonLevelManager.h"

using namespace NCL;
using namespace CSC8503;

HitSphere::HitSphere(reactphysics3d::PhysicsWorld& RP3D_World, Team* team, float radius /*, Weapon* weapon*/) : ToonGameObject(RP3D_World), radius(radius) {
	toDelete = false;
	teamColour = team->getTeamColour();
	this->radius = radius;
}

HitSphere::~HitSphere() {

}

bool HitSphere::CheckDelete() {
	if (toDelete == false) {
		toDelete = true;
		return false;
	}
	return true;
}