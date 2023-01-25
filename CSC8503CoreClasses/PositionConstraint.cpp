#include "PositionConstraint.h"
//#include "../../Common/Vector3.h"
#include "GameObject.h"
#include "PhysicsObject.h"
//#include "Debug.h"



using namespace NCL;
using namespace Maths;
using namespace CSC8503;

PositionConstraint::PositionConstraint(GameObject* a, GameObject* b, float d)
{
	objectA		= a;
	objectB		= b;
	distance	= d;
}

PositionConstraint::~PositionConstraint()
{

}

//a simple constraint that stops objects from being more than <distance> away
//from each other...this would be all we need to simulate a rope, or a ragdoll
void PositionConstraint::UpdateConstraint(float dt)	{
	Vector3 relevantPos = objectA->GetTransform().GetPosition() - objectB->GetTransform().GetPosition();

	float currentDistance = relevantPos.Length();
	float offset = distance - currentDistance;

	if (abs(offset) > 0.0f) {
		Vector3 offsetDir = relevantPos.Normalised();

		PhysicsObject* physA = objectA->GetPhysicsObject();
		PhysicsObject* physB = objectB->GetPhysicsObject();

		Vector3 relativeVelocity = physA->GetLinearVelocity() - physB->GetLinearVelocity();

		float constrainMass = physA->GetInverseMass() + physB->GetInverseMass();

		if (constrainMass > 0.0f) {
			// how much of their relative force is affecting the constraint
			float velocityDot = Vector3::Dot(relativeVelocity, offsetDir);

			float biasFactor = 0.1f;
			float bias = -(biasFactor / dt) * offset;

			float lambda = -(velocityDot + bias) / constrainMass;

			Vector3 aImpulse = offsetDir * lambda;
			Vector3 bImpulse = -offsetDir * lambda;

			physA->ApplyLinearImpulse(aImpulse); // Multiply by mass
			physB->ApplyLinearImpulse(bImpulse); // Multiply by mass
		}
	}
}
