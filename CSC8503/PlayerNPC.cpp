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
		Vector3 euler(0.0f, GetRandomRotation(), 0.0f);
	}
	
	if (!allowInput) return;
	UpdateAnimations();
}

float PlayerNPC::GetRandomRotation()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 360.0f);

	return dis(gen);
}
