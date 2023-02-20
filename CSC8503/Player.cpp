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
    weapon.Update(dt);

	ToonGameObjectAnim::Update(dt);

	//std::cout << "Current Anim: " << currentFrame << std::endl;
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F5))
		renderObject->GetShader()->ReloadShader();

	//TESTING PURPOSE
	/*if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM1)) PlayAnim("Run_N_Aim");
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM2)) PlayAnim("Run_F_Aim");
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM3)) PlayAnim("Run_L_Aim");
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM4)) PlayAnim("Run_R_Aim");
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM5)) PlayAnim("Run_B_Aim");
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM0)) PlayAnim("Idle");*/
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

	float animAngle = RadiansToDegrees(atan2(controls->animDir[1] / 1000.0f, controls->animDir[0] / 1000.0f));
	if (animAngle < 0.0f) animAngle += 360.0f;
	//std::cout << animAngle << std::endl;

	if (isAiming)
	{
		if (isMoving)
		{
			if (animAngle == 0.0f) PlayAnim("Run_R_Aim");
			if (animAngle == 90.0f) PlayAnim("Run_F_Aim");
			if (animAngle == 45.0f) PlayAnim("Run_FR_Aim");
			if (animAngle == 135.0f) PlayAnim("Run_FL_Aim");
			if (animAngle == 180.0f) PlayAnim("Run_L_Aim");
			if (animAngle == 225.0f) PlayAnim("Run_BL_Aim");
			if (animAngle == 270.0f) PlayAnim("Run_B_Aim");
			if (animAngle == 315.0f) PlayAnim("Run_BR_Aim");

			//if(AngleInRange(animAngle, 0, 45.0f)) PlayAnim("Run_")
		}
		else
			PlayAnim("Idle_Aim");
	}
	else
	{
		if (isMoving) PlayAnim("Run_N_Aim");
		else PlayAnim("Idle");
	}

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