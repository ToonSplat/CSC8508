#include "ToonGameObject.h"
#include "RenderObject.h"

NCL::CSC8503::ToonGameObject::ToonGameObject(reactphysics3d::PhysicsWorld& RP3D_World) : physicsWorld(RP3D_World)
{
	reactphysics3d::Transform RP3DTransform;
	reactphysics3d::Vector3 RP3DPosition;
	reactphysics3d::Quaternion RP3DOrientation;

	RP3DPosition.x = transform.GetPosition().x;
	RP3DPosition.y = transform.GetPosition().y;
	RP3DPosition.z = transform.GetPosition().z;

	RP3DOrientation.x = transform.GetOrientation().x;
	RP3DOrientation.y = transform.GetOrientation().y;
	RP3DOrientation.z = transform.GetOrientation().z;
	RP3DOrientation.w = transform.GetOrientation().w;

	RP3DTransform.setPosition(RP3DPosition);
	RP3DTransform.setOrientation(RP3DOrientation);

	rigidBody = physicsWorld.createRigidBody(RP3DTransform);
}

NCL::CSC8503::ToonGameObject::~ToonGameObject()
{
	physicsWorld.destroyRigidBody(rigidBody);

	delete renderObject;
	delete collisionShape;
	delete collider;
}

void NCL::CSC8503::ToonGameObject::SetCollider(reactphysics3d::CollisionShape* RP3D_CollisionShape, const reactphysics3d::Transform& RP3D_Transform)
{
	if (rigidBody == nullptr)
		return;

	rigidBody->addCollider(RP3D_CollisionShape, RP3D_Transform);
}
