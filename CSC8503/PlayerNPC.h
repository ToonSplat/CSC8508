#pragma once
#include "Player.h"

#include "State.h"
#include "StateMachine.h"
#include "StateTransition.h"

#include "NavPathGraphLevel.h"
#include "NavPathFinder.h"

using namespace NCL;
using namespace CSC8503;

class PlayerNPC : public Player
{
public:
	PlayerNPC(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld, Team* team);
	~PlayerNPC();

	virtual void Update(float dt) override;

	static void SetAIShootSpeed(float shootMult) { 
		AI_SHOOT_SPEED = shootMult; 
	}

protected:
	inline float GetRandomJumpTime();

	inline float GetRandomRotation();
	inline float GetRandomRotationTime();

	void GetPath(const Maths::Vector3& from, const Maths::Vector3& to);
	void MoveTowards(const Maths::Vector3& targetPos, const float& dt);

	bool IsStuck(const float& dt);
	bool CanJump();
	bool IsOverNode();

	inline Maths::Vector3 CalcJumpForce();

	float idleTimerCurrent;
	float idleTimerMax;
	float idleTimeCooldown;

	float jumpTimerMin;
	float jumpTimerMax;
	float jumpTimerCurrent;
	float jumpTimerCurrentMax;
	float jumpHeight;

	float rotationTimerMin;
	float rotationTimerMax;
	float rotationTimerCurrent;
	float rotationTimerCurrentMax;	

	float shootDelay;
	float shootDelayMax;
	bool canShoot;

	float roamTimerCurrent;
	float roamTimerMax;

	float stuckTimerCurrent;
	float stuckTimerMax;

	bool hasGotFirstPath;

	float nodeDistanceThreshold;
	int currentNodeIndex;

	NavPathGraphLevel* pathGraph;

	StateMachine* stateMachine;
	State* stateIdle;
	State* stateShooting;
	State* stateGameEnded;

	StateTransition* IdleToShooting;
	StateTransition* ShootingToIdle;
	StateTransition* ShootingToGameEnded;

	StateTransitionFunction IdleToShootingFunc = [&](void)->bool 
	{ 
		return gameWorld->HasGameStarted() && idleTimerCurrent >= idleTimerMax; 
	};

	StateTransitionFunction ShootingToIdleFunc = [&](void)->bool 
	{
		float chance = ToonUtils::RandF(0.0f, 1.0f);
		if (chance <= 0.05f && idleTimerCurrent >= idleTimerMax && idleTimeCooldown <= 0.0f && (isGrounded))
		{
			idleTimerCurrent = 0.0f;
			idleTimeCooldown = 10.0f;
			return true;
		}

		return false;
	};

	StateTransitionFunction ShootingToGameEndedFunc = [&](void)->bool { return !allowInput; };

	NavPathNode* nearestNode;
	Player* targetPlayerTemp;

	std::vector<NavPathNode*> pathNodesList;

	static float AI_SHOOT_SPEED;
};