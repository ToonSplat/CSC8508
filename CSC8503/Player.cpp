#include "Player.h"

using namespace NCL;
using namespace CSC8503;

Player::Player(reactphysics3d::PhysicsWorld& RP3D_World) : ToonGameObject(RP3D_World) {
	team = nullptr;
}

Player::Player(reactphysics3d::PhysicsWorld& RP3D_World, Team* chosenTeam) : ToonGameObject(RP3D_World) {
	team = chosenTeam;
}

Player::~Player() {
	
}

void Player::Update(float dt) {
	SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(reactphysics3d::Vector3(0, ToonGameWorld::Get()->GetMainCamera()->GetYaw() / 180.0f * _Pi, 0)));

	Debug::Print("Sprint:" + std::to_string((int)round((sprintTimer / sprintMax) * 100)) + "%", Vector2(60, 30));

	float fwdForce = moveSpeed;

	if (Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::W) && Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::SHIFT) && sprintTimer > 0.0f) {
		fwdForce = moveSpeed + sprintMulitplier;
		sprintTimer -= dt;
	}
	else if (!Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::SHIFT) && sprintTimer < sprintMax) {
		sprintTimer += dt;
	}

	if (Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::W)) 
		this->GetRigidbody()->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(0, 0, -10.0f));

	if (Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::A)) 
		this->GetRigidbody()->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(-10.0f, 0, 0));
	
	if (Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::D)) 
		this->GetRigidbody()->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(10.0f, 0, 0));
	
	if (Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::S)) 
		this->GetRigidbody()->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(0, 0, 10.0f));

	weapon.Update(dt);
	
	return;
}

void Player::SetWeapon(PaintBallClass* base) {
	weapon = base->MakeInstance();
	std::cout << "WEAPON MADE" << std::endl;
	weapon.SetOwner(this);
	weapon.SetTeam(team);
}

void Player::Shoot() {
	return;
}