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
	delete sampleWeapon;
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

	if (!sampleWeapon) {
		std::cout << "I need a weapon" << std::endl;
		sampleWeapon = new PaintBallClass(15, 500, 0.5f, 1.0f, 5, basicShader, sphereMesh, this);
	}
	else std::cout << "I have my weapon " << sampleWeapon << std::endl;
	if (sampleWeapon)
	{
		std::cout << sampleWeapon << std::endl;
		//sampleWeapon->UpdateTargetObject(targetObject);
		sampleWeapon->Update(dt);
	}
	
	return;
}

void Player::UpdateObjects(ShaderBase* basicShad, MeshGeometry* sphMesh)
{
	basicShader = basicShad;
	sphereMesh  = sphMesh;
}

void Player::Shoot() {
	return;
}