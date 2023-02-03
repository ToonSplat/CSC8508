#include "ToonEventListener.h"
#include "ToonGameWorld.h"

using namespace NCL::CSC8503;

ToonEventListener::ToonEventListener(reactphysics3d::PhysicsWorld* physicsWorld){
	physicsWorld->setEventListener(this);
}
