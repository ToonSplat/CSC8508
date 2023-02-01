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

	collider = rigidBody->addCollider(RP3D_CollisionShape, reactphysics3d::Transform::identity());
}

void NCL::CSC8503::ToonGameObject::SetPosition(const reactphysics3d::Vector3& newPos)
{
	transform.SetPosition(newPos);
	if (rigidBody)
		rigidBody->setTransform(transform.GetR3DTransform());
}

void NCL::CSC8503::ToonGameObject::SetPosition(const Vector3& newPos)
{
	transform.SetPosition(newPos);
	if (rigidBody)
		rigidBody->setTransform(transform.GetR3DTransform());
}

void NCL::CSC8503::ToonGameObject::SetPosition(const float& x, const float& y, const float& z)
{
	Vector3 newPos(x, y, z);
	transform.SetPosition(newPos);
	if (rigidBody)
		rigidBody->setTransform(transform.GetR3DTransform());
}

void NCL::CSC8503::ToonGameObject::SetOrientation(const reactphysics3d::Vector3& newRotEulerAngles)
{
	reactphysics3d::Quaternion newRot = reactphysics3d::Quaternion::fromEulerAngles(newRotEulerAngles);
	transform.SetOrientation(newRot);
	if (rigidBody)
		rigidBody->setTransform(transform.GetR3DTransform());
}

void NCL::CSC8503::ToonGameObject::SetOrientation(const Vector3& newRotEulerAngles)
{
	reactphysics3d::Vector3 newEulerAngles(newRotEulerAngles.x, newRotEulerAngles.y, newRotEulerAngles.z);
	reactphysics3d::Quaternion newRot = reactphysics3d::Quaternion::fromEulerAngles(newEulerAngles);
	transform.SetOrientation(newRot);
	if (rigidBody)
		rigidBody->setTransform(transform.GetR3DTransform());
}

void NCL::CSC8503::ToonGameObject::SetOrientation(const reactphysics3d::Quaternion& newRot)
{
	transform.SetOrientation(newRot);
	if (rigidBody)
		rigidBody->setTransform(transform.GetR3DTransform());
}

void NCL::CSC8503::ToonGameObject::SetOrientation(const Quaternion& newRot)
{
	transform.SetOrientation(newRot);
	if (rigidBody)
		rigidBody->setTransform(transform.GetR3DTransform());
}