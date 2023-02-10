#include "Player.h"
#include "Maths.h"
#include "ToonUtils.h"

using namespace NCL;
using namespace CSC8503;

Player::Player(reactphysics3d::PhysicsWorld& RP3D_World, const Vector3& position, const Vector3& rotationEuler, const float& radius, Team* team) : ToonGameObject(RP3D_World), team(team)
{
	team->AddPlayer();
	isAiming = false;

	moveSpeed = 1500.0f;
	rotationSpeed = 6.0f;
	aimingSpeed = 10.0f;

	transform.SetPosition(position).
		SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z)).
		SetScale(Vector3(radius, radius, radius));

	AddRigidbody();
	rigidBody->setType(reactphysics3d::BodyType::DYNAMIC);
	rigidBody->setLinearDamping(0.8f);
	rigidBody->setAngularLockAxisFactor(reactphysics3d::Vector3(0, 0, 0));
	rigidBody->setIsAllowedToSleep(true);

	reactphysics3d::SphereShape* sphereShape = ToonGameWorld::Get()->GetPhysicsCommon().createSphereShape(radius * 0.85f);
	SetCollisionShape(sphereShape);
	SetCollider(sphereShape);
	SetColliderLayer(ToonCollisionLayer::Character);

	int collisionMask = ToonCollisionLayer::Character | ToonCollisionLayer::Default;
	SetColliderLayerMask(ToonCollisionLayer(collisionMask));

	collider->getMaterial().setBounciness(0.1f);

	ToonGameWorld::Get()->AddGameObject(this);
}

Player::~Player() 
{
	delete team;
}

void Player::Update(float dt)
{
    weapon.Update(dt);
}

void Player::MovementUpdate(float dt, PlayerControl* controls) {
	reactphysics3d::Vector3 linearMovement = reactphysics3d::Vector3(controls->direction[0] / 1000.0f, controls->direction[1] / 1000.0f, controls->direction[2] / 1000.0f);
	linearMovement.normalize();

	isAiming = controls->aiming;
	isMoving = linearMovement.length() >= 0.1f;

	if (controls->aiming)
		targetAngle = controls->camera[1];
	else if (isMoving)
		targetAngle = RadiansToDegrees(atan2(-linearMovement.x, -linearMovement.z));

	reactphysics3d::Quaternion newRot = ToonUtils::ConvertToRP3DQuaternion(Quaternion::EulerAnglesToQuaternion(0, targetAngle, 0));
	SetOrientation(reactphysics3d::Quaternion::slerp(ToonUtils::ConvertToRP3DQuaternion(GetOrientation()), newRot, (controls->aiming ? aimingSpeed : rotationSpeed) * dt));

	if (isMoving)
		rigidBody->applyWorldForceAtCenterOfMass(linearMovement * moveSpeed * dt);
	if(controls->jumping)
		GetRigidbody()->applyWorldForceAtCenterOfMass(reactphysics3d::Vector3(0, 1, 0) * 500.0f);
}

void Player::SetWeapon(PaintBallClass* base) {
	weapon = base->MakeInstance();
	//std::cout << "WEAPON MADE" << std::endl;
	weapon.SetOwner(this);
	weapon.SetTeam(team);
}