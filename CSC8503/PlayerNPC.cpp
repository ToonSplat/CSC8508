#include "PlayerNPC.h"
#include "ToonGame.h"
#include <random>

using namespace NCL;
using namespace CSC8503;

PlayerNPC::PlayerNPC(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld, Team* team) : Player(RP3D_World, gameWorld, team)
{
	allowInput = true;

	jumpTimerMin = 2.0f;
	jumpTimerMax = 7.0f;
	jumpTimerCurrent = 0.0f;
	jumpTimerCurrentMax = GetRandomJumpTime();

	rotationTimerMin = 1.0f;
	rotationTimerMax = 5.0f;
	rotationTimerCurrent = 0.0f;
	rotationTimerCurrentMax = GetRandomRotationTime();

	roamTimerCurrent = 0.0f;
	roamTimerMax = 5.0f;

	stuckTimerCurrent = 0.0f;
	stuckTimerMax = 3.0f;

	nodeDistanceThreshold = 4.0f;

	pathGraph = new NavPathGraphLevel();
	GetPath(GetPosition(), pathGraph->GetRandomNode()->position);

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
				if (jumpTimerCurrent >= jumpTimerMax || IsStuck(dt))
				{
					jumpTimerCurrent = 0.0f;
					jumpTimerMax = GetRandomJumpTime();
					rigidBody->applyWorldForceAtCenterOfMass(reactphysics3d::Vector3(0, 1, 0) * 1000.0f);
				}
			}

			rotationTimerCurrent += dt;
			if (rotationTimerCurrent >= rotationTimerCurrentMax)
			{
				rotationTimerCurrent = 0.0f;
				rotationTimerCurrentMax = GetRandomRotationTime();
				targetAngle = GetRandomRotation();
			}

			reactphysics3d::Quaternion newRot = ToonUtils::ConvertToRP3DQuaternion(NCL::Maths::Quaternion::EulerAnglesToQuaternion(0, targetAngle, 0));
			SetOrientation(reactphysics3d::Quaternion::slerp(ToonUtils::ConvertToRP3DQuaternion(GetOrientation()), newRot, (isAiming ? aimingSpeed : rotationSpeed) * dt));

			//TESTING PURPOSE - FIND PATH TO PLAYER
			/*if (targetPlayerTemp)
			{
				GetPath(targetPlayerTemp->GetPosition(), GetPosition());
				if (pathList.size() > 0)
				{
					for (int i = 1; i < pathList.size(); i++)
						Debug::DrawLine(pathList[i - 1], pathList[i], Debug::YELLOW);

					float nodeDistance = (pathList[currentNodeIndex] - GetPosition()).Length();
					if (nodeDistance <= nodeDistanceThreshold)
					{
						currentNodeIndex = (currentNodeIndex + 1) % pathList.size();
						if (currentNodeIndex >= (int)pathList.size())
							return;
					}

					MoveTowards(pathList[currentNodeIndex], dt);
					Debug::DrawBox(pathList[currentNodeIndex], Vector3(0.4f, 0.4f, 0.4f), Debug::GREEN);
				}
			}*/

			if (pathList.size() > 0)
			{
				for (int i = 1; i < pathList.size(); i++)
					Debug::DrawLine(pathList[i - 1], pathList[i], Debug::YELLOW);

				float nodeDistance = (pathList[currentNodeIndex] - GetPosition()).Length();
				if (nodeDistance <= nodeDistanceThreshold)
				{
					currentNodeIndex++;
					if (currentNodeIndex >= (int)pathList.size())
						GetPath(GetPosition(), pathGraph->GetRandomNode()->position);
				}

				roamTimerCurrent += dt;
				if (roamTimerCurrent >= roamTimerMax)
				{
					roamTimerCurrent = 0.0f;
					GetPath(GetPosition(), pathGraph->GetRandomNode()->position);
				}

				MoveTowards(pathList[currentNodeIndex], dt);
				Debug::DrawBox(pathList[currentNodeIndex], Vector3(0.4f, 0.4f, 0.4f), Debug::GREEN);
				Debug::DrawBox(pathList[pathList.size() - 1], Vector3(0.4f, 0.4f, 0.4f), Debug::GREEN);
			}

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

	targetPlayerTemp = gameWorld->GetToonGame()->GetPlayerFromID(1);	
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
	//pathGraph->DrawDebugPathGraph();

	/*nearestNode = pathGraph->GetNearestNode(GetPosition());
	if (nearestNode)
		Debug::DrawBox(nearestNode->position, Vector3(0.4f, 0.4f, 0.4f), Debug::YELLOW);*/
}

inline float PlayerNPC::GetRandomJumpTime()
{
	return RandF(jumpTimerMin, jumpTimerMax);
}

float PlayerNPC::GetRandomRotation()
{
	return RandF(0, 360);
}

float PlayerNPC::GetRandomRotationTime()
{
	return RandF(rotationTimerMin, rotationTimerMax);
}

void PlayerNPC::GetPath(const Vector3& from, const Vector3& to)
{
	pathList.clear();
	pathList = NavPathFinder::FindPath(*pathGraph, from, to);
	currentNodeIndex = (pathList.size() > 0) ? 0 : -1;
}

void PlayerNPC::MoveTowards(const Vector3& targetPos, const float& dt)
{
	Vector3 dir = (targetPos - GetPosition()).Normalised();
	dir.y = 0.0f;

	rigidBody->applyWorldForceAtCenterOfMass(ToonUtils::ConvertToRP3DVector3(dir) * moveSpeed * dt);
}

bool PlayerNPC::IsStuck(const float& dt)
{
	if (rigidBody->getLinearVelocity().length() <= 3.0f)
	{
		stuckTimerCurrent += dt;
		if (stuckTimerCurrent >= stuckTimerMax)
		{
			jumpTimerCurrent = 0.0f;
			stuckTimerCurrent = 0.0f;
			return true;
		}
	}

	return false;
}

float PlayerNPC::RandF(const float& min, const float& max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(min, max);

	return (float)dis(gen);
}
