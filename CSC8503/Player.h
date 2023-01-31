#pragma once

#include <stack>
#include "GameObject.h"
#include "MeshGeometry.h"
#include "GameTechRenderer.h"

using namespace NCL;
using namespace CSC8503;

#define MIN_FIRE_FORCE 600.0f
#define MAX_FIRE_FORCE 1200.0f

#define MAX_AVAILABLE_FIRE 10

class Player : public GameObject
{
private:
	GameWorld* m_World;
	Vector3		position;
	float		m_PlayerYaw;
	float		m_PlayerPitch;
	bool		m_IsPlayerJumping = false;
	bool		m_IsPlayerRunning = false;
	float		m_MovingSpeed;
	float		m_IsJumpFinished = true;
	const float m_PlayerBaseYPos = 1.5f;
	const float m_PlayerJumpHeight = 2.0f;

	MeshGeometry* m_SphereMesh = nullptr;
	TextureBase* m_BasicTex = nullptr;
	ShaderBase* m_BasicShader = nullptr;

	float		  m_FireForce = MIN_FIRE_FORCE;

	int			  m_firesLeft = MAX_AVAILABLE_FIRE;

	std::stack<GameObject*> m_FireInStack;
	bool					m_HasKey = false;
	float					m_playerHealth = 100.0f;
	Vector3					m_PreviousPosition = Vector3(0.0f, 0.0f, 0.0f);
	GameObject* m_DoorObject;
	GameObject* m_HealthObject;
	GameObject* m_TimerObject;
	std::vector<GameObject*> m_FiresVector;

public:
	Player(MeshGeometry* playerMesh, Vector3 playerPosition, GameTechRenderer* renderer, GameWorld* world, GameObject* doorObject, GameObject* healthObject, GameObject* timerObject);
	~Player();

	void InitializeFire();
	void ResetFire();

	GameObject* GetPlayerObject() { return this; }
	void HandlePlayer(float dt);
	float GetYaw() { return m_PlayerYaw; }
	float GetPitch() { return m_PlayerPitch; }
	void  Fire();
	void Refresh() { m_playerHealth = 100.0f; }

	bool GetIsAlive() { return m_playerHealth > 0.0f; }

	bool HasPlayerMoved()
	{
		Vector3 difference = m_PreviousPosition - GetTransform().GetPosition();
		m_PreviousPosition = GetTransform().GetPosition();
		bool result = IsThereAnyDifferenceInMovement(difference);
		return result;
	}

	void OnCollisionBegin(GameObject* otherObject) override;
	void OnCollisionEnd(GameObject* otherObject) override;
	void IncreaseHealth(float by);

private:
	void CreatePlayer(MeshGeometry* playerMesh, const Vector3 playerPosition, GameTechRenderer* renderer);
	void MakeGoatJump(float speed);
	void UpdateMovingSpeed();
	void UpdateFireForce(float change);
	void PrintData();

	bool IsThereAnyDifferenceInMovement(Vector3 diff)
	{
		bool x = abs(diff.x) > 0.1f;
		bool y = abs(diff.y) > 0.1f;
		bool z = abs(diff.z) > 0.1f;
		return (x || y || z);
	}
};