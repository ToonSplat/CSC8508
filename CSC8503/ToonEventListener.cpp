#include "ToonEventListener.h"
#include "ToonGameWorld.h"
#include "PaintBallProjectile.h"
#include "ToonUtils.h"

using namespace NCL::CSC8503;

ToonEventListener::ToonEventListener(reactphysics3d::PhysicsWorld* physicsWorld) : physicsWorld(physicsWorld) {
	physicsWorld->setEventListener(this);
}

// Override the onContact() method 
void ToonEventListener::onContact(const CollisionCallback::CallbackData& callbackData) {

    // For each contact pair 
    for (reactphysics3d::uint p = 0; p < callbackData.getNbContactPairs(); p++) {

        // Get the contact pair 
        CollisionCallback::ContactPair contactPair = callbackData.getContactPair(p);

        // Check item not yet deleted
        if (!contactPair.getBody1() || !contactPair.getBody2())
            continue;

        // For each contact point of the contact pair 
        for (reactphysics3d::uint c = 0; c < contactPair.getNbContactPoints(); c++) {

            // Get the contact point 
            CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(c);

            // Get the contact point on the first collider and convert it in world-space 
            reactphysics3d::Vector3 worldPoint = contactPair.getCollider1()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider1();
            //std::cout << "CONTACT POINT: " << worldPoint.to_string() << std::endl;
            
        }
#
        // Check if impact involves a paintball
        void* body1 = contactPair.getBody1()->getUserData();
        void* body2 = contactPair.getBody2()->getUserData();
        for (PaintBallProjectile* i : ToonGameWorld::Get()->GetPaintballs()) {
            if (i == body1 || i == body2) {
                // Make the HitSphere
                HitSphere* hitSphere = new HitSphere(*physicsWorld, i->GetTeam(), i->GetRigidbody()->getTransform().getPosition(), i->GetImpactSize());
                // Remove the Paintball
                ToonGameWorld::Get()->RemovePaintball(i);
                ToonGameWorld::Get()->RemoveGameObject(i, false);
                break;
            }
        }

        // Check if collision involves HitSpheres 
        for (HitSphere* i : ToonGameWorld::Get()->GetHitSpheres()) {
            if (i == body1 || i == body2) {                
                for (PaintableObject* p : ToonGameWorld::Get()->GetPaintableObjects()) {
                    if (p == body1 || p == body2) {
                        
                        if (p == body1) {
                            p->AddImpactPoint(ImpactPoint(ToonUtils::ConvertToNCLVector3(i->GetRigidbody()->getTransform().getPosition() - contactPair.getBody1()->getTransform().getPosition()), i->GetTeamColour(), i->GetRadius()));                          
                        }
                        else {
                            p->AddImpactPoint(ImpactPoint(ToonUtils::ConvertToNCLVector3(i->GetRigidbody()->getTransform().getPosition() - contactPair.getBody2()->getTransform().getPosition()), i->GetTeamColour(), i->GetRadius()));
                        }
                    }
                }
            }
        }
      
            
    
        
    }
    for (HitSphere* i : ToonGameWorld::Get()->GetHitSpheres()) {
        if (i->CheckDelete()) {
            //delete the hitsphere
            ToonGameWorld::Get()->RemoveHitSphere(i);
            ToonGameWorld::Get()->RemoveGameObject(i, false);
        }
    }
}

// Override the onTrigger() method
void ToonEventListener::onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData) {

    for (reactphysics3d::uint p = 0; p < callbackData.getNbOverlappingPairs(); p++) {

        reactphysics3d::OverlapCallback::OverlapPair overlapPair = callbackData.getOverlappingPair(p);

        
        std::cout << "HitSphere collisions" << std::endl;
        
        void* body1 = overlapPair.getBody1()->getUserData();

        void* body2 = overlapPair.getBody2()->getUserData();
        for (HitSphere* i : ToonGameWorld::Get()->GetHitSpheres()) {
            if (i == body1 || i == body2) {
                std::cout << "HITSPHERE COLLISION\n";
                break;
            }
        }
    }
}
