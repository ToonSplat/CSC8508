#pragma once
#include "ToonGameObject.h"
#include "Team.h"
#include "Matrix4.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Window.h"
#include "PhysicsObject.h"
#include "Debug.h"
#include <vector>
#include "PaintBallClass.h"
#include "ToonDebugManager.h"

using namespace NCL;
using namespace CSC8503;

class Player : public ToonGameObject {
public:
	Player(reactphysics3d::PhysicsWorld& RP3D_World, const Vector3& position, const Vector3& rotationEuler, const float& radius, Team* team);
	~Player();

	void Update(float dt) override;

	void SetMoveSpeed(float newSpeed) { moveSpeed = newSpeed; }
	float GetMoveSpeed() const { return moveSpeed; }
	void SetSprintMultiplier(float newMultiplier) { sprintMulitplier = newMultiplier; }
	float GetSprintMultiplier() const { return sprintMulitplier; }

	void SetWeapon(PaintBallClass* base);

	void SetDebugManager(ToonDebugManager* tdm) {
		toonDebugManager = tdm;
	}

protected:
	ToonDebugManager* toonDebugManager;

	void Shoot();

	Team* team;

	float moveSpeed;
	float rotationSpeed;
	float aimingSpeed;
	float targetAngle = 0.0f;
	float sprintMax = 2.5f;
	float sprintTimer = 2.0f;
	float sprintMulitplier = 5.0f;

	PaintBallClass weapon;
	bool isAiming, isMoving;
	/*
	* Future Implementations:
	*
	* Weapon* weapon
	* int PercentageCoveredDuringMatch
	* std::vector<Powerups*> appliedPowerups
	*
	*/
};