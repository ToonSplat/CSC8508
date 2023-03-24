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
	bool CheckDrawn();

	Team* GetTeam() const { return team; }
	Maths::Vector3 GetTeamColour() const { return team->GetTeamColour(); }
	float GetRadius() const { return radius; }

protected:
	float radius;
	Team* team;

	bool toDelete;
	bool drawn; 
};