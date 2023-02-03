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

        virtual void onContact(const CollisionCallback::CallbackData& callbackData) override;
        virtual void onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData) override;
        
	};
}