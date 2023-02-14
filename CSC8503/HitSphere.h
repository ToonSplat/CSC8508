#pragma once
#include "ToonGameObject.h"
#include "Team.h"

using namespace NCL;
using namespace CSC8503;

class HitSphere : public ToonGameObject {
public:
	HitSphere(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld, Team* team, float radius);
	~HitSphere();

	bool CheckDelete();

	Vector3 GetTeamColour() const { return teamColour; }
	float GetRadius() const { return radius; }

protected:
	float radius;
	Vector3 teamColour;

	bool toDelete;
};