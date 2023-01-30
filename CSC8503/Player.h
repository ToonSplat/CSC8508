#pragma once
#include "GameObject.h"
#include "Team.h"
#include "Matrix4.h"
#include <vector>

using namespace NCL;
using namespace CSC8503;

class Player : public GameObject {
public:
	Player();
	Player(Team* team);
	~Player();

	void Update(Matrix4 inverseView, float yaw, float pitch, float dt); //All Keyboard inputs done through this; should be called in main game loop e.g. player->Update(...);

protected:
	void Shoot();
	
	Team* team;

	float moveSpeed = 10.0f;
	float sprintMax = 2.5f;
	float sprintTimer = 2.0f;
	int ammoRemaining = 100;

	/*
	* Future Implementations:
	* 
	* Weapon* weapon
	* int PercentageCoveredDuringMatch
	* std::vector<Powerups*> appliedPowerups
	* 
	*/
};
