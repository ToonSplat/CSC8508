#pragma once

#include "ToonEventListener.h"
#include "PaintableObject.h"
#include "ImpactPoint.h"
#include "HitSphere.h"
#include <reactphysics3d/reactphysics3d.h>

namespace NCL::CSC8503 {

	class ToonEventListener : public reactphysics3d::EventListener {
	public:
		ToonEventListener(reactphysics3d::PhysicsWorld* physicsWorld);
		~ToonEventListener() {};

        // Override the onContact() method 
        virtual void onContact(const CollisionCallback::CallbackData& callbackData) override {

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
                    std::cout << "CONTACT POINT: " << worldPoint.to_string() << std::endl;
                    
                    if (dynamic_cast<ToonGameObject*>(contactPair.getBody1()) || dynamic_cast<ToonGameObject*>(contactPair.getBody2())) {
                        // Create a HitSphere here if one object is paintball
                    }
                }
            }
        }

        // Override the onTrigger() method
        virtual void onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData) override {
            
            for (reactphysics3d::uint p = 0; p < callbackData.getNbOverlappingPairs(); p++) {
                
                reactphysics3d::OverlapCallback::OverlapPair overlapPair = callbackData.getOverlappingPair(p);

                if (dynamic_cast<PaintableObject*>(overlapPair.getBody2())) {
                    PaintableObject* object = (PaintableObject*)overlapPair.getBody2();
                    HitSphere* sphere = (HitSphere*)overlapPair.getBody1();
                    object->AddImpactPoint(ImpactPoint(sphere->GetTransform().GetPosition(), sphere->GetTeamColour(), 5));
                }
            }
        }
	};
}