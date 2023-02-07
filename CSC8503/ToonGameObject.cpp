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
	SetColliderLayer(ToonCollisionLayer::Default);

	int everythingMask = ToonCollisionLayer::Default | ToonCollisionLayer::Character;
	SetColliderLayerMask(ToonCollisionLayer(everythingMask));
}

void NCL::CSC8503::ToonGameObject::SetPosition(const reactphysics3d::Vector3& newPos)
{
	rigidBody->setTransform(reactphysics3d::Transform(newPos, rigidBody->getTransform().getOrientation()));
}

void NCL::CSC8503::ToonGameObject::SetPosition(const Vector3& newPos)
{
	SetPosition(reactphysics3d::Vector3(newPos.x, newPos.y, newPos.z));
}

void NCL::CSC8503::ToonGameObject::SetPosition(const float& x, const float& y, const float& z)
{
	SetPosition(reactphysics3d::Vector3(x, y, z));
}

void NCL::CSC8503::ToonGameObject::SetOrientation(const reactphysics3d::Vector3& newRotVec3)
{
	SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(newRotVec3));
}

void NCL::CSC8503::ToonGameObject::SetOrientation(const Vector3& newRotVec3)
{
	SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(reactphysics3d::Vector3(newRotVec3.x, newRotVec3.y, newRotVec3.z)));
}

void NCL::CSC8503::ToonGameObject::SetOrientation(const reactphysics3d::Quaternion& newRotQuat)
{
	rigidBody->setTransform(reactphysics3d::Transform(rigidBody->getTransform().getPosition(), newRotQuat));
}

void NCL::CSC8503::ToonGameObject::SetOrientation(const Quaternion& newRotQuat)
{
	SetOrientation(reactphysics3d::Quaternion(newRotQuat.x, newRotQuat.y, newRotQuat.z, newRotQuat.w));
}