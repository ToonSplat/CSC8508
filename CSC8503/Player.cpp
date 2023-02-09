#include "Player.h"
#include "Maths.h"
#include "ToonUtils.h"

using namespace NCL;
using namespace CSC8503;

Player::Player(reactphysics3d::PhysicsWorld& RP3D_World, const Vector3& position, const Vector3& rotationEuler, const float& radius, Team* team) : ToonGameObject(RP3D_World), team(team)
{
	team = nullptr;
	isAiming = false;

	moveSpeed = 1500.0f;
	rotationSpeed = 6.0f;
	aimingSpeed = 10.0f;

	transform.SetPosition(position).
		SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotationEuler.x, rotationEuler.y, rotationEuler.z)).
		SetScale(Vector3(radius, radius, radius));

	AddRigidbody();
	rigidBody->setType(reactphysics3d::BodyType::DYNAMIC);
	rigidBody->setLinearDamping(0.8f);
	rigidBody->setAngularLockAxisFactor(reactphysics3d::Vector3(0, 0, 0));
	rigidBody->setIsAllowedToSleep(true);

	reactphysics3d::SphereShape* sphereShape = ToonGameWorld::Get()->GetPhysicsCommon().createSphereShape(radius * 0.85f);
	SetCollisionShape(sphereShape);
	SetCollider(sphereShape);
	SetColliderLayer(ToonCollisionLayer::Character);

	int collisionMask = ToonCollisionLayer::Character | ToonCollisionLayer::Default;
	SetColliderLayerMask(ToonCollisionLayer(collisionMask));

	collider->getMaterial().setBounciness(0.1f);

	ToonGameWorld::Get()->AddGameObject(this);
}

Player::~Player() 
{
	delete team;
}

void Player::Update(float dt)
{
	isAiming = Window::GetMouse()->ButtonHeld(MouseButtons::RIGHT);

	Vector3 forward = ToonGameWorld::Get()->GetMainCamera()->GetForward();
	Vector3 right = ToonGameWorld::Get()->GetMainCamera()->GetRight();
	Vector3 up = ToonGameWorld::Get()->GetMainCamera()->GetUp();

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
	rigidBody->setTransform(newRotTransform);

	if (isMoving)
		rigidBody->applyWorldForceAtCenterOfMass(ToonUtils::ConvertToRP3DVector3(linearMovement.Normalised()) * moveSpeed * dt);
    
    weapon.Update(dt);
}

void Player::SetWeapon(PaintBallClass* base) {
	weapon = base->MakeInstance();
	//std::cout << "WEAPON MADE" << std::endl;
	weapon.SetOwner(this);
	weapon.SetTeam(team);
}