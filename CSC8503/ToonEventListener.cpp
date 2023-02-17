#include "ToonEventListener.h"
#include "ToonGameWorld.h"
#include "PaintBallProjectile.h"
#include "ToonUtils.h"
#include "PaintableObject.h"
#include "ImpactPoint.h"
#include "HitSphere.h"
#include "ToonLevelManager.h"
#include "ToonGameWorld.h"
#include "ToonNetworkedGame.h"

using namespace NCL::CSC8503;

ToonEventListener::ToonEventListener(reactphysics3d::PhysicsWorld* physicsWorld, ToonGameWorld* gameWorld, ToonLevelManager* levelManager) : 
    physicsWorld(physicsWorld), gameWorld(gameWorld), levelManager(levelManager) {
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
        for (PaintBallProjectile* i : gameWorld->GetPaintballs()) {
            if (i == body1 || i == body2) {
                //make an audio play on collision
                i->GetAudioEmitter()->SetTarget(ToonUtils::ConvertToNCLVector3(i->GetRigidbody()->getTransform().getPosition()));
                AudioSystem::GetAudioSystem()->AddSoundEmitter(i->GetAudioEmitter());

                // Make the HitSphere if local play or the server
                if(gameWorld->GetNetworkStatus() != NetworkingStatus::Client)
                    levelManager->AddHitSphereToWorld(i->GetRigidbody()->getTransform().getPosition(), i->GetImpactSize(), i->GetTeam());
                // Remove the Paintball
                gameWorld->RemovePaintball(i);
                gameWorld->RemoveGameObject(i, false);
                break;
            }
        }

        // Check if collision involves HitSpheres 
        for (HitSphere* i : gameWorld->GetHitSpheres()) {
            if (i == body1 || i == body2) {
                for (PaintableObject* p : gameWorld->GetPaintableObjects()) {
                    if (p == body1 || p == body2) {
                        Vector3 localPosition;
                        localPosition = ToonUtils::ConvertToNCLVector3(i->GetRigidbody()->getTransform().getPosition() -
                            p->GetRigidbody()->getTransform().getPosition());
                        p->AddImpactPoint(ImpactPoint(localPosition, i->GetTeam(), i->GetRadius()));
                        if (server)
                            server->SendImpactPoint(ImpactPoint(localPosition, i->GetTeam(), i->GetRadius()), p);
                    }
                }
            }
        }
      
            
    
        
    }
    for (HitSphere* i : gameWorld->GetHitSpheres()) {
        if (i->CheckDelete()) {
            //delete the hitsphere
            gameWorld->RemoveHitSphere(i);
            gameWorld->RemoveGameObject(i, false);
        }
    }
}

// Override the onTrigger() method
void ToonEventListener::onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData) {

    for (reactphysics3d::uint p = 0; p < callbackData.getNbOverlappingPairs(); p++) {

        reactphysics3d::OverlapCallback::OverlapPair overlapPair = callbackData.getOverlappingPair(p);

        
        std::cout << "TRIGGER HitSphere collisions" << std::endl;
        
        void* body1 = overlapPair.getBody1()->getUserData();
        void* body2 = overlapPair.getBody2()->getUserData();
        for (HitSphere* i : gameWorld->GetHitSpheres()) {
            if (i == body1 || i == body2) {
                std::cout << "TRIGGER HITSPHERE COLLISION\n";
                break;
            }
        }
    }
}
