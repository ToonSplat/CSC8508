#include "PaintBallProjectile.h"
#include "ToonGameWorld.h"
#include "ToonLevelManager.h"

using namespace NCL::CSC8503;

PaintBallProjectile::PaintBallProjectile(reactphysics3d::PhysicsWorld& RP3D_World, const reactphysics3d::Vector3& position,
	const reactphysics3d::Vector3& rotationEuler, const float& radius, const float& _impactSize) : ToonGameObject(RP3D_World), impactSize(_impactSize)
{
	GetTransform().SetPosition(position).
		SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z)).
		SetScale(Vector3(radius, radius, radius));

	SetRenderObject(new ToonRenderObject(&GetTransform(), ToonLevelManager::Get()->GetMesh("sphere"), ToonLevelManager::Get()->GetTexture("basic"), ToonLevelManager::Get()->GetShader("basic")));

	AddRigidbody();
	GetRigidbody()->setType(reactphysics3d::BodyType::DYNAMIC);
	GetRigidbody()->setLinearDamping(0.66);
	GetRigidbody()->setAngularDamping(0.1);
	GetRigidbody()->setMass(0.1);

	reactphysics3d::SphereShape* sphereShape = ToonGameWorld::Get()->GetPhysicsCommon().createSphereShape(radius);
	SetCollisionShape(sphereShape);
	SetCollider(sphereShape);
	GetCollider()->getMaterial().setBounciness(0.1f);

	ToonGameWorld::Get()->AddGameObject(this);
}

NCL::CSC8503::PaintBallProjectile::~PaintBallProjectile()
{
}
