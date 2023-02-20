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
	if (!LoadAnim(&meshAnims["Idle_Aim"], "Boss_Gun_Idle_Aim.anm")) return;
	if (!LoadAnim(&meshAnims["Run_N_Aim"], "Boss_Gun_Run_N_Aim.anm")) return;
	if (!LoadAnim(&meshAnims["Run_F_Aim"], "Boss_Gun_Run_F_Aim.anm")) return;
	if (!LoadAnim(&meshAnims["Run_FL_Aim"], "Boss_Gun_Run_FL_Aim.anm")) return;
	if (!LoadAnim(&meshAnims["Run_FR_Aim"], "Boss_Gun_Run_FR_Aim.anm")) return;
	if (!LoadAnim(&meshAnims["Run_L_Aim"], "Boss_Gun_Run_L_Aim.anm")) return;
	if (!LoadAnim(&meshAnims["Run_R_Aim"], "Boss_Gun_Run_R_Aim.anm")) return;
	if (!LoadAnim(&meshAnims["Run_B_Aim"], "Boss_Gun_Run_B_Aim.anm")) return;
	if (!LoadAnim(&meshAnims["Run_BL_Aim"], "Boss_Gun_Run_BL_Aim.anm")) return;
	if (!LoadAnim(&meshAnims["Run_BR_Aim"], "Boss_Gun_Run_BR_Aim.anm")) return;
	PlayAnim("Idle");
}

Player::~Player() 
{
	team->RemovePlayer();
}

bool Player::WeaponUpdate(float dt, PlayerControl* controls)
{
    return weapon.Update(dt, controls);
}

void Player::MovementUpdate(float dt, PlayerControl* controls) {

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F5))
		renderObject->GetShader()->ReloadShader();

	reactphysics3d::Vector3 linearMovement = reactphysics3d::Vector3(controls->direction[0] / 1000.0f, controls->direction[1] / 1000.0f, controls->direction[2] / 1000.0f);
	linearMovement.normalize();

	isMoving = linearMovement.length() >= 0.1f;
	isAiming = controls->aiming;

	if (isAiming) {
		targetAngle = controls->camera[1];
	}
	else if (isMoving)
		targetAngle = RadiansToDegrees(atan2(-linearMovement.x, -linearMovement.z));

	float animAngle = RadiansToDegrees(atan2(controls->animDir[1] / 1000.0f, controls->animDir[0] / 1000.0f));

	if (animAngle < 0.0f) animAngle += 360.0f;
	//std::cout << animAngle << std::endl;

	reactphysics3d::Quaternion newRot = ToonUtils::ConvertToRP3DQuaternion(Quaternion::EulerAnglesToQuaternion(0, targetAngle, 0));
	SetOrientation(reactphysics3d::Quaternion::slerp(ToonUtils::ConvertToRP3DQuaternion(GetOrientation()), newRot, (controls->aiming ? aimingSpeed : rotationSpeed) * dt));


	if (isMoving)
		rigidBody->applyWorldForceAtCenterOfMass(linearMovement * moveSpeed * dt);
	if (controls->jumping) {
		GetRigidbody()->applyWorldForceAtCenterOfMass(reactphysics3d::Vector3(0, 1, 0) * 500.0f);
		controls->jumping = false;
	}
}

void Player::AnimationUpdate(float dt) {
	ToonGameObjectAnim::Update(dt);
	reactphysics3d::Vector3 linVel = GetRigidbody()->getLinearVelocity();
	linVel = GetRigidbody()->getTransform().getInverse().getOrientation() * linVel;
	linVel.y = 0;
	isMoving = linVel.length() >= 0.5f;
	linVel.normalize();
	if (isAiming)
	{
		if (isMoving)
		{
			if (linVel.z < -0.5) {
				if (linVel.x > 0.1)
					PlayAnim("Run_FR_Aim");
				else if (linVel.x < -0.1)
					PlayAnim("Run_FL_Aim");
				else PlayAnim("Run_F_Aim");
			}
			else if (linVel.z > 0.5) {
				if (linVel.x > 0.1)
					PlayAnim("Run_BR_Aim");
				else if (linVel.x < -0.1)
					PlayAnim("Run_BL_Aim");
				else PlayAnim("Run_B_Aim");
			}
			else if (linVel.x > 0.5)
				PlayAnim("Run_R_Aim");
			else if (linVel.x < -0.5)
				PlayAnim("Run_L_Aim");
			else {
				std::cout << "How did we get here?\n";
				PlayAnim("Idle_Aim");
			}
		}
		else
			PlayAnim("Idle_Aim");
	}
	else
	{
		if (isMoving) PlayAnim("Run_N_Aim");
		else PlayAnim("Idle");
	}
}

void Player::SetWeapon(PaintBallClass* base) {
	weapon = base->MakeInstance();
	//std::cout << "WEAPON MADE" << std::endl;
	weapon.SetOwner(this);
	weapon.SetTeam(team);
}