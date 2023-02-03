#include "ToonEventListener.h"
#include "ToonGameWorld.h"
#include "PaintBallProjectile.h"

using namespace NCL::CSC8503;

ToonEventListener::ToonEventListener(reactphysics3d::PhysicsWorld* physicsWorld){
	physicsWorld->setEventListener(this);
}

// Override the onContact() method 
void ToonEventListener::onContact(const CollisionCallback::CallbackData& callbackData) {

    // For each contact pair 
    for (reactphysics3d::uint p = 0; p < callbackData.getNbContactPairs(); p++) {

        // Get the contact pair 
        CollisionCallback::ContactPair contactPair = callbackData.getContactPair(p);

        // For each contact point of the contact pair 
        for (reactphysics3d::uint c = 0; c < contactPair.getNbContactPoints(); c++) {

            // Get the contact point 
            CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(c);

            // Get the contact point on the first collider and convert it in world-space 
            reactphysics3d::Vector3 worldPoint = contactPair.getCollider1()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider1();
            //std::cout << "CONTACT POINT: " << worldPoint.to_string() << std::endl;
            if (contactPair.getBody1()->getUserData() == ToonGameWorld::Get()->GetUserData()[0] || contactPair.getBody2()->getUserData() == ToonGameWorld::Get()->GetUserData()[0]) {
                std::cout << "PAINTBALL HIT" << std::endl;
            }
        }
    }
}

// Override the onTrigger() method
void ToonEventListener::onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData) {

    for (reactphysics3d::uint p = 0; p < callbackData.getNbOverlappingPairs(); p++) {

        reactphysics3d::OverlapCallback::OverlapPair overlapPair = callbackData.getOverlappingPair(p);

        if (dynamic_cast<PaintableObject*>(overlapPair.getBody2())) {
            PaintableObject* object = (PaintableObject*)overlapPair.getBody2();
            HitSphere* sphere = (HitSphere*)overlapPair.getBody1();
            object->AddImpactPoint(ImpactPoint(sphere->GetTransform().GetPosition(), sphere->GetTeamColour(), 5));
        }
    }
}
