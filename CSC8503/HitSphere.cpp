#include "HitSphere.h"
#include "PaintableObject.h"

using namespace NCL;
using namespace CSC8503;

HitSphere::HitSphere() {

}

HitSphere::~HitSphere() {

}

void HitSphere::OnCollisionBegin(GameObject* otherObject) { //Must be changed to onTrigger at some point
	if (dynamic_cast<PaintableObject*>(otherObject)) {
		PaintableObject* p = (PaintableObject*)otherObject;
		p->AddImpactPoint(ImpactPoint(this->GetTransform().GetPosition(), Vector3(1, 0, 0), 5)); //Default colour red; make sure to change
	}
}
