#include "Player.h"
#include "Maths.h"
#include "ToonUtils.h"

using namespace NCL;
using namespace CSC8503;

Player::Player(reactphysics3d::PhysicsWorld& RP3D_World, const Vector3& position, const Vector3& rotationEuler, const float& radius, Team* team) : ToonGameObject(RP3D_World), team(team)
{
	team = nullptr;
	isAiming = false;

	moveSpeed = 30.0f;
	rotationSpeed = 6.0f;
	aimingSpeed = 10.0f;

	GetTransform().SetPosition(position).
		SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z)).
		SetScale(Vector3(radius, radius, radius));

	AddRigidbody();
	GetRigidbody()->setType(reactphysics3d::BodyType::DYNAMIC);
	GetRigidbody()->setLinearDamping(0.8f);
	GetRigidbody()->setAngularLockAxisFactor(reactphysics3d::Vector3(0, 0, 0));
	GetRigidbody()->setIsAllowedToSleep(true);

	reactphysics3d::SphereShape* sphereShape = ToonGameWorld::Get()->GetPhysicsCommon().createSphereShape(radius * 0.85f);
	SetCollisionShape(sphereShape);
	SetCollider(sphereShape);
	GetCollider()->getMaterial().setBounciness(0.1f);

	ToonGameWorld::Get()->AddGameObject(this);
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
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::W)) linearMovement += forward;
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::S)) linearMovement -= forward;

	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::A)) linearMovement -= right;
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::D)) linearMovement += right;

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) GetRigidbody()->applyWorldForceAtCenterOfMass(reactphysics3d::Vector3(0, 1, 0) * 500.0f);
	
	isMoving = linearMovement.Length() >= 0.1f;
	if (isAiming) targetAngle = ToonGameWorld::Get()->GetMainCamera()->GetYaw();

	//targetAngle = ToonGameWorld::Get()->GetMainCamera()->GetYaw();
	if (!isAiming && isMoving) targetAngle = RadiansToDegrees(atan2(-linearMovement.Normalised().x, -linearMovement.Normalised().z));

	//else if (!isAiming && isMoving)
		//targetAngle = RadiansToDegrees(atan2(-linearMovement.x, -linearMovement.z)) + ToonGameWorld::Get()->GetMainCamera()->GetYaw();
	
	Quaternion newRotNCL = Quaternion::EulerAnglesToQuaternion(0, targetAngle, 0);
	reactphysics3d::Quaternion newRot(newRotNCL.x, newRotNCL.y, newRotNCL.z, newRotNCL.w);
	reactphysics3d::Transform newRotTransform(GetRigidbody()->getTransform().getPosition(), reactphysics3d::Quaternion::slerp(GetRigidbody()->getTransform().getOrientation(), newRot, (isAiming ? aimingSpeed : rotationSpeed) * dt));
	GetRigidbody()->setTransform(newRotTransform);

	if (isMoving)
		GetRigidbody()->applyWorldForceAtCenterOfMass(ToonUtils::ConvertToRP3DVector3(linearMovement.Normalised()) * moveSpeed);
    
    weapon.Update(dt);
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