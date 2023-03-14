#pragma once
#include "Player.h"

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

	float jumpTimerCurrent;
	float jumpTimerMax;

	float rotationTimerCurrent;
	float rotationTimerMax;
};