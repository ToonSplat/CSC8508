#include "PlayerNPC.h"
#include <random>

using namespace NCL;
using namespace CSC8503;

PlayerNPC::PlayerNPC(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld, Team* team) : Player(RP3D_World, gameWorld, team)
{
	allowInput = true;

	jumpTimerCurrent = 0.0f;
	jumpTimerMax = 6.0f;

	rotationTimerMin = 1.0f;
	rotationTimerMax = 5.0f;
	rotationTimerCurrent = 0.0f;
	rotationTimerCurrentMax = GetRandomRotationTime();

	pathGraph = new NavPathGraphLevel();
	std::vector<Vector3> paths = NavPathFinder::FindPath(*pathGraph, pathGraph->GetNode(8)->position, pathGraph->GetNode(14)->position);

	stateMachine = new StateMachine();
	stateIdle = new State([&](float dt)->void
		{
		});

	stateShooting = new State([&](float dt)->void
		{
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
			if (rotationTimerCurrent >= rotationTimerCurrentMax)
			{
				rotationTimerCurrent = 0.0f;
				targetAngle = GetRandomRotation();
				rotationTimerCurrentMax = GetRandomRotationTime();
			}

			reactphysics3d::Quaternion newRot = ToonUtils::ConvertToRP3DQuaternion(NCL::Maths::Quaternion::EulerAnglesToQuaternion(0, targetAngle, 0));
			SetOrientation(reactphysics3d::Quaternion::slerp(ToonUtils::ConvertToRP3DQuaternion(GetOrientation()), newRot, (isAiming ? aimingSpeed : rotationSpeed) * dt));

			UpdateMovementAnimations();
		});

	stateGameEnded = new State([&](float dt)->void
		{
		});

	IdleToShooting = new StateTransition(stateIdle, stateShooting, IdleToShootingFunc);
	ShootingToGameEnded = new StateTransition(stateShooting, stateGameEnded, ShootingToGameEndedFunc);

	stateMachine->AddState(stateIdle);
	stateMachine->AddState(stateShooting);
	stateMachine->AddState(stateGameEnded);
	stateMachine->AddTransition(IdleToShooting);
	stateMachine->AddTransition(ShootingToGameEnded);
}

PlayerNPC::~PlayerNPC()
{
	team->RemovePlayer();

	delete stateIdle;
	delete stateShooting;
	delete stateGameEnded;

	delete IdleToShooting;
	delete ShootingToGameEnded;

	delete stateMachine;

	delete pathGraph;
}

void PlayerNPC::Update(float dt)
{
	ToonGameObjectAnim::Update(dt);
	stateMachine->Update(dt);
	pathGraph->DrawDebugPath();
}

float PlayerNPC::GetRandomRotation()
{
	return RandF(0, 360);
}

float PlayerNPC::GetRandomRotationTime()
{
	return RandF(rotationTimerMin, rotationTimerMax);
}

float PlayerNPC::RandF(const float& min, const float& max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);

	return (float)dis(gen);
}
