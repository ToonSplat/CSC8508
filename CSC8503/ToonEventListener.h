#pragma once

#include "ToonEventListener.h"
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
                    //std::cout << "CONTACT POINT: " << worldPoint.to_string() << std::endl;
                }
            }
        }
	};
}