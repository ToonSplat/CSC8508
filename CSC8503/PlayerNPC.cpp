#include "PlayerNPC.h"
#include "ToonGame.h"
#include "ToonUtils.h"
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
	jumpHeight = 25.0f;

	rotationTimerMin = 0.5f;
	rotationTimerMax = 2.0f;
	rotationTimerCurrent = 0.0f;
	rotationTimerCurrentMax = GetRandomRotationTime();

	roamTimerCurrent = 0.0f;
	roamTimerMax = 5.0f;	

	shootDelay = 0.0f;
	shootDelayMax = 2.0f;
	canShoot = false;

	//weapon.ownerIsNPC = true;

	stuckTimerCurrent = 0.0f;
	stuckTimerMax = 3.0f;

	nodeDistanceThreshold = 4.0f;

	pathGraph = new NavPathGraphLevel();	

	stateMachine = new StateMachine();
	stateIdle = new State([&](float dt)->void
		{
		});

	stateShooting = new State([&](float dt)->void
		{
			if (!hasGotFirstPath)
			{
				GetPath(GetPosition(), pathGraph->GetRandomNode()->position);
				hasGotFirstPath = true;
			}

			reactphysics3d::Vector3 linVel = GetRigidbody()->getLinearVelocity();
			linVel = GetRigidbody()->getTransform().getInverse().getOrientation() * linVel;
			isMoving = linVel.length() >= 0.5f;

			linVel.y = 0;
			linVel.normalize();

			isAiming = true;
			isGrounded = IsGrounded();

			rigidBody->setLinearDamping(isGrounded ? 5.0f : 0.8f);
			moveSpeed = isGrounded ? 5000.0f : 1500.0f;

			if (!allowInput) return;
			if (isGrounded)
			{
				jumpTimerCurrent += dt;
				if (jumpTimerCurrent >= jumpTimerMax && CanJump())
				{
					jumpTimerCurrent = 0.0f;
					jumpTimerMax = GetRandomJumpTime();
					rigidBody->applyWorldForceAtCenterOfMass(reactphysics3d::Vector3(0, 1, 0) * 1500.0f);
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

			if (pathNodesList.size() > 0)
			{
				for (int i = 1; i < pathNodesList.size(); i++)
					Debug::DrawLine(pathNodesList[i - 1]->position, pathNodesList[i]->position, Debug::YELLOW);

				float nodeDistance = (pathNodesList[currentNodeIndex]->position - GetPosition()).Length();
				//std::cout << "Product: " << Vector3::Dot((pathList[currentNodeIndex] - GetPosition()), Vector3(0, 1, 0)) << std::endl;
				if (nodeDistance <= nodeDistanceThreshold || IsOverNode())
				{
					currentNodeIndex++;
					if (currentNodeIndex >= (int)pathNodesList.size())
						GetPath(GetPosition(), pathGraph->GetRandomNode()->position);
				}

				if (isGrounded)
				{
					if ((pathNodesList[currentNodeIndex]->isJumpNode) && pathNodesList[currentNodeIndex]->position.y > GetPosition().y + 3.0f)
					{
						jumpTimerCurrent = jumpTimerCurrentMax * 0.5f;	//Reset it back so it doesn't jump immediately after landing
						rigidBody->setLinearVelocity(ToonUtils::ConvertToRP3DVector3(CalcJumpForce()));
					}
				}

				if (IsStuck(dt))
					GetPath(GetPosition(), pathGraph->GetRandomNode()->position);

				MoveTowards(pathNodesList[currentNodeIndex]->position, dt);

				Debug::DrawBox(GetPosition() + Vector3(0, 3.0f, 0), Vector3(0.4f, 0.4f, 0.4f), Debug::CYAN);
				Debug::DrawBox(pathNodesList[currentNodeIndex]->position, Vector3(0.4f, 0.4f, 0.4f), Debug::GREEN);
				Debug::DrawBox(pathNodesList[pathNodesList.size() - 1]->position, Vector3(0.4f, 0.4f, 0.4f), Debug::RED);
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
	delete stateMachine;
	delete pathGraph;
}

void PlayerNPC::Update(float dt)
{
	ToonGameObjectAnim::Update(dt);
	stateMachine->Update(dt);

	if (!canShoot)
	{
		shootDelay += dt;
		if (shootDelay >= shootDelayMax)
			canShoot = true;
	}

	if (canShoot) weapon.NPCUpdate(dt);
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
	pathNodesList.clear();
	pathNodesList = NavPathFinder::FindPath(*pathGraph, from, to);
	currentNodeIndex = (pathNodesList.size() > 0) ? 0 : -1;
}

void PlayerNPC::MoveTowards(const Vector3& targetPos, const float& dt)
{
	Vector3 dir = (targetPos - GetPosition()).Normalised();
	dir.y = 0.0f;

	rigidBody->applyWorldForceAtCenterOfMass(ToonUtils::ConvertToRP3DVector3(dir) * moveSpeed * dt);
}

bool PlayerNPC::IsStuck(const float& dt)
{
	if (!rigidBody) return false;

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

bool PlayerNPC::CanJump()
{
	//Jump like an idiot anyway
	if ((int)pathNodesList.size() <= 0 || currentNodeIndex == -1)
		return true;

	if (currentNodeIndex + 1 >= (int)pathNodesList.size())
		return false;

	NavPathNode* nodeCurrent = pathNodesList[currentNodeIndex];	
	NavPathNode* nodeNext = pathNodesList[currentNodeIndex + 1];

	if (nodeCurrent != nullptr && nodeNext != nullptr)
	{
		if (nodeCurrent->isJumpNode || nodeNext->isJumpNode)
		{
			//std::cout << "NO: Can't Jump because the current or next node requires JUMP Calculations!" << std::endl;
			return false;
		}
	}

	//std::cout << "YES: Can JUMP!" << std::endl;
	return true;
}

bool PlayerNPC::IsOverNode()
{
	if (currentNodeIndex == -1 || (int)pathNodesList.size() <= 0)
		return false;

	Vector3 nodePos = pathNodesList[currentNodeIndex]->position;
	Vector3 nodePosUp = nodePos + Vector3(0, 3.0f, 0);
	Vector3 nodeUp = (nodePos - nodePosUp).Normalised();

	Vector3 posDir = (nodePos - GetPosition()).Normalised();
	float angle = Vector3::Dot(nodeUp, posDir);
	
	return (angle >= 0.8f) && pathNodesList[currentNodeIndex]->position.y < GetPosition().y + 3.0f;
}

Vector3 PlayerNPC::CalcJumpForce()
{
	float gravity = -9.81f;

	Vector3 nodePosition = pathNodesList[currentNodeIndex]->position;
	float displacementY = nodePosition.y - GetPosition().y;
	Vector3 displacementXZ = Vector3(nodePosition.x - GetPosition().x, 0.0f, nodePosition.z - GetPosition().z);

	Vector3 velocityY = Vector3(0, 1.0f, 0) * sqrtf(-2.0f * gravity * jumpHeight);
	Vector3 velocityXZ = displacementXZ / (sqrtf(-2.0f * jumpHeight / gravity) + sqrtf(2.0f * (displacementY - jumpHeight) / gravity));

	return velocityXZ + velocityY;
}

float PlayerNPC::RandF(const float& min, const float& max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(min, max);

	return (float)dis(gen);
}
