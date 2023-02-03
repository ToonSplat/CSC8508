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

using namespace NCL;
using namespace CSC8503;

class Player : public ToonGameObject {
public:
	Player(reactphysics3d::PhysicsWorld& RP3D_World, const Vector3& position, const Vector3& rotationEuler, const float& radius);
	Player(reactphysics3d::PhysicsWorld& RP3D_World, Team* chosenTeam);
	~Player();

	void Update(float dt);
	void Update(Matrix4& inverseView, float& yaw, float& pitch, float dt); //All Keyboard inputs done through this; should be called in main game loop e.g. player->Update(...);

	void SetMoveSpeed(float newSpeed) { moveSpeed = newSpeed; }
	float GetMoveSpeed() const { return moveSpeed; }
	void SetSprintMultiplier(float newMultiplier) { sprintMulitplier = newMultiplier; }
	float GetSprintMultiplier() const { return sprintMulitplier; }

	void UpdateTargetObject(ToonGameObject* targetObj) { targetObj = targetObject; }
	void UpdateObjects(ShaderBase* basicShad, MeshGeometry* sphMesh);

protected:
	void Shoot();

	Team* team;

	float moveSpeed;
	float rotationSpeed;
	float aimingSpeed;
	float targetAngle = 0.0f;
	float sprintMax = 2.5f;
	float sprintTimer = 2.0f;
	float sprintMulitplier = 5.0f;
	int ammoRemaining = 100;

	ToonGameObject* targetObject = nullptr;
	PaintBallClass* sampleWeapon = nullptr;
	ShaderBase* basicShader;
	MeshGeometry* sphereMesh;

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