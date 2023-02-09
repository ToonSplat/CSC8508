#pragma once
#include "ToonGameObject.h"
#include "Team.h"

using namespace NCL;
using namespace CSC8503;

class HitSphere : public ToonGameObject {
public:
	HitSphere(reactphysics3d::PhysicsWorld& RP3D_World, Team* team, reactphysics3d::Vector3 position, float radius /*, Weapon weapon*/);
	~HitSphere();

	void OnCollisionBegin(ToonGameObject* otherObject);

	bool CheckDelete();

	Vector3 GetTeamColour() const { return teamColour; }
	float GetRadius() const { return radius; }

protected:
	float radius;
	Vector3 teamColour;

	bool toDelete; //Time left before removed from game world.

	/*
	* Eventually when weapons are added:
	* 
	* float weaponRadius
	*/
};