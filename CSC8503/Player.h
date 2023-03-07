#pragma once
#include "ToonGameObjectAnim.h"
#include "Team.h"
#include "Matrix4.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Window.h"
#include "Debug.h"
#include <vector>
#include "PaintBallClass.h"
#include "PlayerControl.h"

using namespace NCL;
using namespace CSC8503;

class Player : public ToonGameObjectAnim {
public:
	Player(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld, Team* team);
	~Player();

	bool WeaponUpdate(float dt, PlayerControl* controls);
	void MovementUpdate(float dt, PlayerControl* controls);
	virtual void Update(float dt) override;

	void SetMoveSpeed(float newSpeed) { moveSpeed = newSpeed; }
	float GetMoveSpeed() const { return moveSpeed; }
	void SetSprintMultiplier(float newMultiplier) { sprintMulitplier = newMultiplier; }
	float GetSprintMultiplier() const { return sprintMulitplier; }

	bool IsAiming() const { return isAiming; }
	void SetAiming(bool isAiming) { this->isAiming = isAiming; }
	bool IsMoving() const { return rigidBody ? rigidBody->getLinearVelocity().length() > 0.1f : false; }

	Team* GetTeam() const { return team; }
	PaintBallClass GetWeapon() { return weapon; }
	void SetWeapon(PaintBallClass* base);

protected:
	bool AngleInRange(const float& val, const float& min, const float& max) { return min <= val && val <= max; }
	bool IsGrounded();

protected:
	Team* team;
	Camera* camera;

	float moveSpeed;
	float rotationSpeed;
	float aimingSpeed;
	float targetAngle = 0.1f;
	float sprintMax = 2.5f;
	float sprintTimer = 2.0f;
	float sprintMulitplier = 5.0f;

	PaintBallClass weapon;
	bool isAiming, isMoving, isGrounded;
	/*
	* Future Implementations:
	*
	* Weapon* weapon
	* int PercentageCoveredDuringMatch
	* std::vector<Powerups*> appliedPowerups
	*
	*/
};