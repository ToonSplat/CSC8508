#include "Player.h"
#include "SphereVolume.h"
#include "RenderObject.h"
#include "PhysicsObject.h"

using namespace NCL::CSC8503;

Player::Player(MeshGeometry* playerMesh, Vector3 playerPosition, GameTechRenderer* renderer, GameWorld* world, GameObject* doorObject, GameObject* healthObject, GameObject* timerObject) : m_DoorObject(doorObject), m_HealthObject(healthObject), m_TimerObject(timerObject)
{
	m_MovingSpeed = 10.0f;
	m_World = world;
	CreatePlayer(playerMesh, playerPosition, renderer);
	m_SphereMesh = renderer->LoadMesh("sphere.msh");
	m_BasicTex = renderer->LoadTexture("checkerboard.png");
	m_BasicShader = renderer->LoadShader("scene.vert", "scene.frag");
	for (int i = 0; i < MAX_AVAILABLE_FIRE; i++)
	{
		InitializeFire();
	}
}

Player::~Player()
{
}

void Player::InitializeFire()
{
	float radius = 0.1f;
	GameObject* sphere = new GameObject("Fire");

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius + 0.5f);
	sphere->SetBoundingVolume((CollisionVolume*)volume);
	Vector3 firePosition = Vector3(-100.0f, -100.0f, -100.0f);//(position.x, position.y + 0.1f, position.z);

	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(firePosition);
	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), m_SphereMesh, /*m_BasicTex*/nullptr, m_BasicShader));
	float objectElasticity = 0.0f;//(float)(rand() % 100 + 1) / 110.0f;
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(20.0f);
	sphere->GetRenderObject()->SetColour(Vector4(0.0f, 1.0f, 1.0f, 1.0f));
	sphere->GetPhysicsObject()->InitSphereInertia();
	m_FireInStack.push(sphere);
	m_FiresVector.push_back(sphere);
	m_World->AddGameObject(sphere);
}

void Player::ResetFire()
{
	m_FireInStack.empty();
	for (auto obj : m_FiresVector)
	{
		obj->GetTransform().SetPosition(Vector3(-100.0f, -100.0f, -100.0f));
		m_FireInStack.push(obj);
	}
	m_firesLeft = MAX_AVAILABLE_FIRE;
}


void Player::CreatePlayer(MeshGeometry* playerMesh, const Vector3 playerPosition, GameTechRenderer* renderer) {
	float meshSize = 1.0f;
	float inverseMass = 0.5f;
	name = "";
	SphereVolume* volume = new SphereVolume(1.0f);

	ShaderBase* basicShader = renderer->LoadShader("scene.vert", "scene.frag");

	SetBoundingVolume((CollisionVolume*)volume);

	GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(playerPosition);

	SetRenderObject(new RenderObject(&GetTransform(), playerMesh, nullptr, basicShader));
	SetPhysicsObject(new PhysicsObject(&GetTransform(), GetBoundingVolume()));

	GetPhysicsObject()->SetInverseMass(inverseMass);
	GetPhysicsObject()->InitSphereInertia();
	GetTransform().SetOrientation((const NCL::Maths::Quaternion&)Matrix4::Rotation(45, Vector3(0, -1, 0)));// Matrix4::Rotation(45, Vector3(0, -1, 0)));
	GetRenderObject()->SetColour(Vector4(1.0f, 1.0f, 0.0f, 1.0f));
	m_World->AddGameObject(this);
}

void Player::HandlePlayer(float dt)
{
	m_IsPlayerRunning = false;
	m_PlayerPitch -= (Window::GetMouse()->GetRelativePosition().y);
	m_PlayerYaw -= (Window::GetMouse()->GetRelativePosition().x);

	m_PlayerPitch = min(m_PlayerPitch, 90.0f);
	m_PlayerYaw = max(m_PlayerYaw, -90.0f);

	if (m_PlayerYaw < 0.0f) { m_PlayerYaw += 360.0f; }
	if (m_PlayerYaw > 360.0f) { m_PlayerYaw -= 360.0f; }

	float frameSpeed = m_MovingSpeed * dt;

	position = GetTransform().GetPosition();

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W)) {
		position += Matrix4::Rotation(m_PlayerYaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * frameSpeed;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S)) {
		position -= Matrix4::Rotation(m_PlayerYaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * frameSpeed;
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
		position += Matrix4::Rotation(m_PlayerYaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * frameSpeed;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
		position -= Matrix4::Rotation(m_PlayerYaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * frameSpeed;
	}
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::UP)) { UpdateFireForce(5.0f); }
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::DOWN)) { UpdateFireForce(-5.0f); }
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE) && m_IsJumpFinished)
	{
		m_IsJumpFinished = false;
		m_IsPlayerJumping = true;
	}
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::SHIFT)) { m_IsPlayerRunning = true; }
	if (m_firesLeft > 0 && (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F) || Window::GetMouse()->ButtonPressed(NCL::MouseButtons::LEFT))) { Fire(); }
	UpdateMovingSpeed();
	MakeGoatJump(frameSpeed);

	GetTransform().SetOrientation((const NCL::Maths::Quaternion&)Matrix4::Rotation(m_PlayerYaw, Vector3(0, 1, 0)));

	GetTransform().SetPosition(position);

	PrintData();
}

void Player::MakeGoatJump(float speed)
{
	if (position.y >= m_PlayerBaseYPos + m_PlayerJumpHeight) { m_IsPlayerJumping = false; }
	if (m_IsPlayerJumping) { position.y += speed; }
	if (position.y < m_PlayerBaseYPos) { m_IsJumpFinished = true; }
}

void Player::UpdateMovingSpeed()
{
	m_MovingSpeed = m_IsPlayerRunning ? 15.0f : 10.0f;
}

void Player::UpdateFireForce(float change)
{
	m_FireForce += change;
	if (m_FireForce > MAX_FIRE_FORCE) { m_FireForce = MAX_FIRE_FORCE; }
	if (m_FireForce < MIN_FIRE_FORCE) { m_FireForce = MIN_FIRE_FORCE; }
}

void Player::Fire()
{
	m_firesLeft--;
	GameObject* fireObject = m_FireInStack.top();
	m_FireInStack.pop();
	fireObject->GetTransform().SetPosition(Vector3(position.x, position.y + 0.1f, position.z));
	Vector3 forceInDirection = Matrix4::Rotation(GetYaw(), Vector3(0, 1, 0)) * Matrix4::Rotation(GetPitch(), Vector3(1, 0, 0)) * Vector3(0, 0, -1) * m_FireForce;
	fireObject->GetPhysicsObject()->AddForce(forceInDirection);
	fireObject->GetPhysicsObject()->InitSphereInertia();
}

void Player::PrintData()
{
	Debug::Print("Fire Force : " + std::to_string(m_FireForce), Vector2(5, 95), Debug::BLUE);
	Debug::Print("Available Fire : " + std::to_string(m_firesLeft), Vector2(5, 92), m_firesLeft < 5 ? Debug::RED : Debug::BLUE);
	Debug::Print("Health : " + std::to_string((int)m_playerHealth), Vector2(75, 5), m_playerHealth < 50 ? Debug::RED : Debug::BLUE);
}

void Player::OnCollisionBegin(GameObject* otherObject)
{
	/*CollisionDetection::CollisionInfo info;
	if ((otherObject->GetName() == "Key") && CollisionDetection::ObjectIntersection(this, otherObject, info))
	{
		otherObject->GetTransform().SetPosition(Vector3(-100.0f, -100.0f, -100.0f));
		if (KeyGameObject* keyGameObject = (KeyGameObject*)otherObject)
		{
			m_GameStates->UpdateScore(keyGameObject->GetPoints());
		}
		m_GameStates->m_HasKey = true;
		m_DoorObject->GetPhysicsObject()->SetInverseMass(0.5f);
		m_GameStates->m_remainingCoins--;
	}
	else if (otherObject->GetName() == "Coin" && CollisionDetection::ObjectIntersection(this, otherObject, info))
	{
		otherObject->GetTransform().SetPosition(Vector3(-100.0f, -100.0f, -100.0f));
		if (KeyGameObject* keyGameObject = (KeyGameObject*)otherObject)
		{
			m_GameStates->UpdateScore(keyGameObject->GetPoints());
			m_GameStates->m_remainingCoins--;
		}
	}
	else if ((otherObject->GetName() == "Maze Enemy" || otherObject->GetName() == "Outside Maze Enemy" || otherObject->GetName() == "Keeper Enemy") && CollisionDetection::ObjectIntersection(this, otherObject, info))
	{
		EnemyStateGameObject* enemy = (EnemyStateGameObject*)otherObject;
		Vector3 direction = info.point.normal * -50;
		GetPhysicsObject()->ApplyLinearImpulse(direction);
		otherObject->GetPhysicsObject()->ApplyLinearImpulse(-direction);
		if (enemy != NULL && enemy->GetIsAlive()) { m_playerHealth -= otherObject->GetName() == "Maze Enemy" ? 100.0f : 2.0f; }
		if (m_playerHealth <= 0.0f)
		{
			m_GameStates->m_CurrentState = GameState::GameOver;
		}
	}
	else if (otherObject->GetName() == "Health" && CollisionDetection::ObjectIntersection(this, otherObject, info))
	{
		m_HealthObject->GetTransform().SetPosition(Vector3(-100.0f, -100.0f, -100.0f));
		IncreaseHealth(10.0f);
	}
	else if (otherObject->GetName() == "Timer" && CollisionDetection::ObjectIntersection(this, otherObject, info))
	{
		m_TimerObject->GetTransform().SetPosition(Vector3(-100.0f, -100.0f, -100.0f));
		if (m_GameStates->m_RemainingTime + 20.0f <= 300.0f)
		{
			m_GameStates->m_RemainingTime += 20.0f;
		}
		else
		{
			m_GameStates->m_RemainingTime = 300.0f;
		}
	}*/
}

void Player::OnCollisionEnd(GameObject* otherObject)
{
	/*CollisionDetection::CollisionInfo info;
	if (otherObject->GetName() == "Fire" && CollisionDetection::ObjectIntersection(this, otherObject, info))
	{
		m_firesLeft++;
		otherObject->GetTransform().SetPosition(Vector3(-100.0f, -100.0f, -100.0f));
		m_FireInStack.push(otherObject);
	}*/
}

void Player::IncreaseHealth(float by)
{
	float newHealth = m_playerHealth + by;
	m_playerHealth = newHealth > 100.0f ? 100.0f : newHealth;
}