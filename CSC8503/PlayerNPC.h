#pragma once
#include "Player.h"
#include "State.h"
#include "StateMachine.h"
#include "StateTransition.h"

using namespace NCL;
using namespace CSC8503;

class PlayerNPC : public Player
{
public:
	PlayerNPC(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld, Team* team);
	~PlayerNPC();

	virtual void Update(float dt) override;

protected:
	float GetRandomRotation();
	float GetRandomRotationTime();

	float jumpTimerCurrent;
	float jumpTimerMax;

	float rotationTimerMin;
	float rotationTimerMax;
	float rotationTimerCurrent;
	float rotationTimerCurrentMax;

	StateMachine* stateMachine;
	State* stateIdle;
	State* stateShooting;
	State* stateGameEnded;

	StateTransition* IdleToShooting;
	StateTransition* ShootingToGameEnded;

	StateTransitionFunction IdleToShootingFunc = [&](void)->bool { return gameWorld->HasGameStarted(); };
	StateTransitionFunction ShootingToGameEndedFunc = [&](void)->bool { return !allowInput; };

private:
	float RandF(const float& min, const float& max);
};