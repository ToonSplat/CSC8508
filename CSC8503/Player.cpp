#include "Player.h"

using namespace NCL;
using namespace CSC8503;

Player::Player() {
	team = nullptr;
}

Player::Player(Team* chosenTeam) {
	team = chosenTeam;
}

Player::~Player() {

}

void Player::Update(Matrix4& inverseView, float& yaw, float& pitch, float dt) {
	Matrix4 horizontalRotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0));
	Matrix4 verticalRotation = Matrix4::Rotation(pitch, Vector3(1, 0, 0));
	Matrix4 combinedRotation = horizontalRotation * verticalRotation;
	this->GetTransform().SetOrientation(combinedRotation);

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
		sprintTimer = max(sprintTimer, sprintMax);
	}

	if (Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::W))
		this->GetPhysicsObject()->AddForce(fwdAxis * fwdForce);

	if (Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::A))
		this->GetPhysicsObject()->AddForce(-rightAxis * moveSpeed);

	if (Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::D))
		this->GetPhysicsObject()->AddForce(rightAxis * moveSpeed);

	if (Window::GetKeyboard()->KeyHeld(NCL::KeyboardKeys::S))
		this->GetPhysicsObject()->AddForce(-fwdAxis * moveSpeed);

	if (!sampleWeapon) { sampleWeapon = new PaintBallClass(15, 500, 0.5f, 1.0f, 5, world, basicShader, sphereMesh, this); }
	if (sampleWeapon)
	{
		sampleWeapon->UpdateTargetObject(targetObject, world, m_CollisionAt);
		sampleWeapon->Update(dt);
	}

	return;
}

void Player::UpdateObjects(ShaderBase* basicShad, MeshGeometry* sphMesh, GameWorld* wld)
{
	basicShader = basicShad;
	sphereMesh = sphMesh;
	world = wld;
}

void Player::Shoot() {
	return;
}