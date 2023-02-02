#include "Player.h"
#include "Maths.h"
#include "ToonUtils.h"

using namespace NCL;
using namespace CSC8503;

Player::Player(reactphysics3d::PhysicsWorld& RP3D_World) : ToonGameObject(RP3D_World) {
	team = nullptr;
	isAiming = false;
}

Player::Player(reactphysics3d::PhysicsWorld& RP3D_World, Team* chosenTeam) : ToonGameObject(RP3D_World) {
	team = chosenTeam;
	isAiming = false;
}

Player::~Player() {

}

void Player::Update(float dt)
{
	isAiming = Window::GetMouse()->ButtonHeld(MouseButtons::RIGHT);

	Matrix4 rotation = Matrix4::Rotation(ToonGameWorld::Get()->GetMainCamera()->GetYaw(), Vector3(0, 1, 0)) * Matrix4::Rotation(ToonGameWorld::Get()->GetMainCamera()->GetPitch(), Vector3(1, 0, 0));
	Vector3 forward = rotation * Vector3(0, 0, -1.0f);
	Vector3 right = rotation * Vector3(1, 0, 0);
	Vector3 up = rotation * Vector3(0, 1, 0);

	//linearMovement.z = -1.0f;
		/*targetAngle = ToonGameWorld::Get()->GetMainCamera()->GetYaw();
		if (!isAiming) targetAngle -= 90.0f;*/

	Vector3 linearMovement;	
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::W)) linearMovement += forward * 10.0f;
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::S)) linearMovement -= forward * 10.0f;

	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::A)) linearMovement -= right * 10.0f;
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::D)) linearMovement += right * 10.0f;


	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) GetRigidbody()->applyWorldForceAtCenterOfMass(reactphysics3d::Vector3(0, 1, 0) * 500.0f);
	
	isMoving = linearMovement.Length() >= 0.1f;
	if (isAiming) targetAngle = ToonGameWorld::Get()->GetMainCamera()->GetYaw();

	//targetAngle = ToonGameWorld::Get()->GetMainCamera()->GetYaw();
	if (!isAiming && isMoving) targetAngle = RadiansToDegrees(atan2(-linearMovement.Normalised().x, -linearMovement.Normalised().z));

	//else if (!isAiming && isMoving)
		//targetAngle = RadiansToDegrees(atan2(-linearMovement.x, -linearMovement.z)) + ToonGameWorld::Get()->GetMainCamera()->GetYaw();
	
	Quaternion newRotNCL = Quaternion::EulerAnglesToQuaternion(0, targetAngle, 0);
	reactphysics3d::Quaternion newRot(newRotNCL.x, newRotNCL.y, newRotNCL.z, newRotNCL.w);
	reactphysics3d::Transform newRotTransform(GetRigidbody()->getTransform().getPosition(), reactphysics3d::Quaternion::slerp(GetRigidbody()->getTransform().getOrientation(), newRot, 5.0f * dt));
	GetRigidbody()->setTransform(newRotTransform);

	if (isMoving)
		GetRigidbody()->applyWorldForceAtCenterOfMass(ToonUtils::ConvertToRP3DVector3(linearMovement.Normalised()) * 10.0f);
}

void Player::Update(Matrix4& inverseView, float& yaw, float& pitch, float dt)
{
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
	/*if (!sampleWeapon) { sampleWeapon = new PaintBallClass(15, 500, 0.5f, 1.0f, 5, basicShader, sphereMesh, this); }
	if (sampleWeapon)
	{
		sampleWeapon->UpdateTargetObject(targetObject);
		sampleWeapon->Update(dt);
	}*/
	
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