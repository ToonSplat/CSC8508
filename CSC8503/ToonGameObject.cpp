#include "ToonGameObject.h"
#include "ToonRenderObject.h"

NCL::CSC8503::ToonGameObject::ToonGameObject(reactphysics3d::PhysicsWorld& RP3D_World) : physicsWorld(RP3D_World)
{
	worldID = -1;
	isActive = true;

	rigidBody = nullptr;
	renderObject = nullptr;
	collisionShape = nullptr;
	collider = nullptr;
}

NCL::CSC8503::ToonGameObject::~ToonGameObject()
{
	physicsWorld.destroyRigidBody(rigidBody);

	delete renderObject;
	delete collisionShape;
	delete collider;
}

void NCL::CSC8503::ToonGameObject::AddRigidbody()
{
	rigidBody = physicsWorld.createRigidBody(transform.GetR3DTransform());
}

void NCL::CSC8503::ToonGameObject::SetCollider(reactphysics3d::CollisionShape* RP3D_CollisionShape)
{
	if (rigidBody == nullptr)
		return;

	rigidBody->addCollider(RP3D_CollisionShape, transform.GetR3DTransform());
}
