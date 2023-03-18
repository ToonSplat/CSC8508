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

protected:
	inline float GetRandomJumpTime();

	inline float GetRandomRotation();
	inline float GetRandomRotationTime();

	void GetPath(const Vector3& from, const Vector3& to);
	void MoveTowards(const Vector3& targetPos, const float& dt);

	bool IsStuck(const float& dt);

	float jumpTimerMin;
	float jumpTimerMax;
	float jumpTimerCurrent;
	float jumpTimerCurrentMax;

	float rotationTimerMin;
	float rotationTimerMax;
	float rotationTimerCurrent;
	float rotationTimerCurrentMax;

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
	StateTransition* ShootingToGameEnded;

	StateTransitionFunction IdleToShootingFunc = [&](void)->bool { return gameWorld->HasGameStarted(); };
	StateTransitionFunction ShootingToGameEndedFunc = [&](void)->bool { return !allowInput; };

	NavPathNode* nearestNode;
	Player* targetPlayerTemp;

	std::vector<Vector3> pathList;

private:
	float RandF(const float& min, const float& max);
};