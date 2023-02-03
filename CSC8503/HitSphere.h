#pragma once
#include "GameObject.h"
#include "Team.h"

using namespace NCL;
using namespace CSC8503;

class HitSphere : public GameObject {
public:
	HitSphere(Team* team /*, Weapon weapon*/);
	~HitSphere();

	void OnCollisionBegin(GameObject* otherObject);

	void Update(float dt);

	Vector3 GetTeamColour() const { return teamColour; }

protected:
	Vector3 teamColour;

	float lifetime; //Time left before removed from game world.

	/*
	* Eventually when weapons are added:
	* 
	* float weaponRadius
	*/
};