#pragma once
#include "GameObject.h"
#include "Team.h"
#include "Matrix4.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Window.h"
#include "PhysicsObject.h"
#include "Debug.h"
#include <vector>
#include "AudioSystem.h"

using namespace NCL;
using namespace CSC8503;

class Player : public GameObject {
public:
	Player();
	Player(Team* team);
	~Player();

	void Update(Matrix4& inverseView, float& yaw, float& pitch, float dt); //All Keyboard inputs done through this; should be called in main game loop e.g. player->Update(...);

	void SetMoveSpeed(float newSpeed) { moveSpeed = newSpeed; }
	float GetMoveSpeed() const { return moveSpeed; }
	void SetSprintMultiplier(float newMultiplier) { sprintMulitplier = newMultiplier; }
	float GetSprintMultiplier() const { return sprintMulitplier; }

protected:
	void Shoot();
	
	Team* team;

	float moveSpeed = 10.0f;
	float sprintMax = 2.5f;
	float sprintTimer = 2.0f;
	float sprintMulitplier = 5.0f;
	int ammoRemaining = 100;

	AudioSystem* audioSystem;
	/*
	* Future Implementations:
	* 
	* Weapon* weapon
	* int PercentageCoveredDuringMatch
	* std::vector<Powerups*> appliedPowerups
	* 
	*/
};
