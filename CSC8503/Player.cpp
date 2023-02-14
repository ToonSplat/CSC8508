#include "Player.h"
#include "Maths.h"
#include "ToonUtils.h"

using namespace NCL;
using namespace CSC8503;

Player::Player(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld, Team* team) : ToonGameObjectAnim(RP3D_World, gameWorld), team(team)
{
	team->AddPlayer();
	isAiming = false;

	moveSpeed = 1500.0f;
	rotationSpeed = 6.0f;
	aimingSpeed = 10.0f;

	if (!LoadAnim(&meshAnims["Idle"], "Boss_Gun_Idle.anm")) return;
	PlayAnim("Idle");
}

Player::~Player() 
{
	delete team;
}

void Player::Update(float dt)
{
    weapon.Update(dt);

	ToonGameObjectAnim::Update(dt);

	//std::cout << "Current Anim: " << currentFrame << std::endl;
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F5))
		renderObject->GetShader()->ReloadShader();
}

void Player::MovementUpdate(float dt, PlayerControl* controls) {
	reactphysics3d::Vector3 linearMovement = reactphysics3d::Vector3(controls->direction[0] / 1000.0f, controls->direction[1] / 1000.0f, controls->direction[2] / 1000.0f);
	linearMovement.normalize();

	isMoving = linearMovement.length() >= 0.1f;
	isAiming = controls->aiming;

	if (isAiming) {
		targetAngle = controls->camera[1];
	}
	else if (isMoving)
		targetAngle = RadiansToDegrees(atan2(-linearMovement.x, -linearMovement.z));

	reactphysics3d::Quaternion newRot = ToonUtils::ConvertToRP3DQuaternion(Quaternion::EulerAnglesToQuaternion(0, targetAngle, 0));
	SetOrientation(reactphysics3d::Quaternion::slerp(ToonUtils::ConvertToRP3DQuaternion(GetOrientation()), newRot, (controls->aiming ? aimingSpeed : rotationSpeed) * dt));

	if (isMoving)
		rigidBody->applyWorldForceAtCenterOfMass(linearMovement * moveSpeed * dt);
	if (controls->jumping) {
		GetRigidbody()->applyWorldForceAtCenterOfMass(reactphysics3d::Vector3(0, 1, 0) * 500.0f);
		controls->jumping = false;
	}
}

void Player::SetWeapon(PaintBallClass* base) {
	weapon = base->MakeInstance();
	//std::cout << "WEAPON MADE" << std::endl;
	weapon.SetOwner(this);
	weapon.SetTeam(team);
}