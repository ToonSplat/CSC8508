#include "HitSphere.h"
#include "PaintableObject.h"

using namespace NCL;
using namespace CSC8503;

HitSphere::HitSphere(Team* team /*, Weapon* weapon*/) {
	lifetime = 1.5f;
	teamColour = team->getTeamColour();

	/*
	* weaponRadius = weapon->GetRadius();
	*/
}

HitSphere::~HitSphere() {

}

void HitSphere::Update(float dt) {
	if (lifetime <= 0) {
		this->isActive = false;
		//Remove from game world
	}
	lifetime -= dt;
}

void HitSphere::OnCollisionBegin(GameObject* otherObject) { //Must be changed to onTrigger at some point
	if (dynamic_cast<PaintableObject*>(otherObject)) {
		PaintableObject* p = (PaintableObject*)otherObject;
		p->AddImpactPoint(ImpactPoint(this->GetTransform().GetPosition(), teamColour, 5)); //radius to weapon radius
	}
}
