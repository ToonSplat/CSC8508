#include "PaintBallProjectile.h"
#include "ToonGameWorld.h"

PaintBallProjectile::PaintBallProjectile(reactphysics3d::PhysicsWorld& RP3D_World, const Vector3& position,
	const Vector3& rotationEuler, const float& radius) : ToonGameObject(RP3D_World) {
	GetTransform().SetPosition(position).
		SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z)).
		SetScale(Vector3(radius, radius, radius));

	AddRigidbody();
	GetRigidbody()->setType(reactphysics3d::BodyType::DYNAMIC);
	GetRigidbody()->setLinearDamping(0.8f);
	GetRigidbody()->setAngularLockAxisFactor(reactphysics3d::Vector3(1, 1, 1));
	GetRigidbody()->setIsAllowedToSleep(false);

	reactphysics3d::SphereShape* sphereShape = ToonGameWorld::Get()->GetPhysicsCommon().createSphereShape(radius * 0.85f);
	SetCollisionShape(sphereShape);
	SetCollider(sphereShape);
	GetCollider()->getMaterial().setBounciness(0.1f);

	GetRigidbody()->setType(reactphysics3d::BodyType::DYNAMIC);
	GetRigidbody()->setLinearDamping(0.66);
	GetRigidbody()->setAngularDamping(0.66);
	GetRigidbody()->setMass(0.1);

	ToonGameWorld::Get()->AddGameObject(this);
}
