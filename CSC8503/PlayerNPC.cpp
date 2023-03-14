#include "PlayerNPC.h"
#include <random>

PlayerNPC::PlayerNPC(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld, Team* team) : Player(RP3D_World, gameWorld, team)
{
	allowInput = true;

	jumpTimerCurrent = 0.0f;
	jumpTimerMax = 6.0f;

	rotationTimerCurrent = 0.0f;
	rotationTimerMax = 2.0f;
}

PlayerNPC::~PlayerNPC()
{
	team->RemovePlayer();
}

void PlayerNPC::Update(float dt)
{
	ToonGameObjectAnim::Update(dt);

	reactphysics3d::Vector3 linVel = GetRigidbody()->getLinearVelocity();
	linVel = GetRigidbody()->getTransform().getInverse().getOrientation() * linVel;
	isMoving = linVel.length() >= 0.5f;
	
	linVel.y = 0;
	linVel.normalize();

	isAiming = true;
	isGrounded = IsGrounded();

	if (!allowInput) return;
	if (isGrounded)
	{
		jumpTimerCurrent += dt;
		if (jumpTimerCurrent >= jumpTimerMax)
		{
			jumpTimerCurrent = 0.0f;
			GetRigidbody()->applyWorldForceAtCenterOfMass(reactphysics3d::Vector3(0, 1, 0) * 1000.0f);
		}
	}

	rotationTimerCurrent += dt;
	if (rotationTimerCurrent >= rotationTimerMax)
	{
		rotationTimerCurrent = 0.0f;
		targetAngle = GetRandomRotation();
	}

	reactphysics3d::Quaternion newRot = ToonUtils::ConvertToRP3DQuaternion(NCL::Maths::Quaternion::EulerAnglesToQuaternion(0, targetAngle, 0));
	SetOrientation(reactphysics3d::Quaternion::slerp(ToonUtils::ConvertToRP3DQuaternion(GetOrientation()), newRot, (isAiming ? aimingSpeed : rotationSpeed) * dt));
	
	UpdateAnimations();
}

float PlayerNPC::GetRandomRotation()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis((float)0.0f, (float)360.0f);

	return (float)dis(gen);
}
