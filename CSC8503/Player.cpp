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

void Player::Update(Matrix4& inverseView, float& yaw, float& pitch, float dt) {
	Matrix4 horizontalRotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0));
	Matrix4 verticalRotation = Matrix4::Rotation(pitch, Vector3(1, 0, 0));
	Matrix4 combinedRotation = horizontalRotation * verticalRotation;
	this->GetTransform().SetOrientation(combinedRotation);
	this->GetRigidbody()->setTransform(transform.GetR3DTransform());

	Vector3 rightAxis = Vector3(inverseView.GetColumn(0)); 

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();

	Debug::Print("Sprint:" + std::to_string((int)round((sprintTimer / sprintMax) * 100)) + "%", Vector2(60, 30));

	float fwdForce = moveSpeed;

	if (Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::W) && Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::SHIFT) && sprintTimer > 0.0f) {
		fwdForce = moveSpeed + sprintMulitplier;
		sprintTimer -= dt;
	}
	else if (!Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::SHIFT) && sprintTimer < sprintMax) {
		sprintTimer += dt;
	}

	if (Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::W)) {
		this->GetRigidbody()->applyWorldForceAtCenterOfMass(reactphysics3d::Vector3(0, 10.0f, 0));
		//this->GetPhysicsObject()->AddForce(fwdAxis * fwdForce);
	}
	if (Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::A)) {
		//this->GetPhysicsObject()->AddForce(-rightAxis * moveSpeed);
	}
	if (Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::D)) {
		//this->GetPhysicsObject()->AddForce(rightAxis * moveSpeed);
	}
	if (Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::S)) {
		//this->GetPhysicsObject()->AddForce(-fwdAxis * moveSpeed);
	}
	if (!sampleWeapon) { sampleWeapon = new PaintBallClass(15, 500, 0.5f, 1.0f, 5, basicShader, sphereMesh, this); }
	if (sampleWeapon)
	{
		sampleWeapon->UpdateTargetObject(targetObject);
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