#include "PaintBallClass.h"
#include "RenderObject.h"
#include "PhysicsObject.h"

using namespace NCL;
using namespace CSC8503;

PaintBallClass::PaintBallClass(int _maxAmmoInUse, int _maxAmmoHeld, int _fireRate, int _reloadTime, float _maxShootDist, GameWorld* world, ShaderBase* basicShader, MeshGeometry* sphereMesh, GameObject* cameraTargetObject) {
	ammoInUse = _maxAmmoInUse;
	ammoHeld = _maxAmmoHeld;
	maxAmmoHeld = _maxAmmoHeld;
	maxAmmoInUse = _maxAmmoInUse;
	fireRate = _fireRate;
	reloadTime = _reloadTime;
	maxShootDistance = _maxShootDist;
	m_World = world;
	m_BasicShader = basicShader;
	m_SphereMesh = sphereMesh;
	m_CameraTargetObject = cameraTargetObject;

	shootTimer = 0.0f;
	reloadTimer = 0.0f;
}

PaintBallClass::~PaintBallClass() {

}

void PaintBallClass::Update(float dt) {
	//if left mouse status
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::LEFT) && ammoInUse > 0)
		status = isFiring;
	else if (ammoInUse <= 0 || Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::R))
		status = isReloading;
	else
		status = isIdle;

	switch (status) {
	case isIdle:
		break;
	case isFiring:
		Shoot(dt);
		break;
	case isReloading:
		Reload(dt);
		break;
	}
}

void PaintBallClass::Shoot(float dt) {
	shootTimer += dt;
	if (shootTimer >= fireRate && ammoInUse > 0) {
		// Shoot Projectile here
		CreateBullet();
		std::cout << "Weapon is shooting" << std::endl;
		ammoInUse--;
		shootTimer = 0.0f;
	}
}

void PaintBallClass::Reload(float dt) {
	if (ammoInUse < maxAmmoInUse && ammoHeld > 0) {
		reloadTimer += dt;
		if (reloadTimer >= reloadTime) {
			//Reload Maths here
			if ((ammoHeld + ammoInUse) < maxAmmoInUse) {
				ammoInUse += ammoHeld;
				ammoHeld = 0;
			}
			else {
				int temp = maxAmmoInUse - ammoInUse;
				ammoInUse += temp;
				ammoHeld -= temp;
			}
			std::cout << "Ammo: " << ammoInUse << "/" << ammoHeld << std::endl;
		}
	}
}

void PaintBallClass::PickUpAmmo(int amt) {

}



void PaintBallClass::CreateBullet()
{
	GameObject* sphereBullet = new GameObject("Fire");
	float		  radius = 0.1f;
	Vector3		  sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphereBullet->SetBoundingVolume((CollisionVolume*)volume);

	sphereBullet->GetTransform().SetScale(sphereSize);
	sphereBullet->SetRenderObject(new RenderObject(&sphereBullet->GetTransform(), m_SphereMesh, nullptr, m_BasicShader));
	sphereBullet->SetPhysicsObject(new PhysicsObject(&sphereBullet->GetTransform(), sphereBullet->GetBoundingVolume()));

	sphereBullet->GetPhysicsObject()->SetInverseMass(100.0f);
	sphereBullet->GetRenderObject()->SetColour(Vector4(0.0f, 1.0f, 1.0f, 1.0f));
	sphereBullet->GetPhysicsObject()->InitSphereInertia();
	m_World->AddGameObject(sphereBullet);


	Vector3 position = m_CameraTargetObject->GetTransform().GetPosition();
	Vector3 direction = (Matrix4::Rotation(m_World->GetMainCamera()->GetYaw(), Vector3(0, 1, 0)) * Matrix4::Rotation(m_World->GetMainCamera()->GetPitch(), Vector3(1, 0, 0)) * Vector3(0, 0, -1));
	if (m_TargetObjet)
	{
		direction = GetDirection(m_CameraTargetObject->GetTransform().GetPosition() + Vector3(0, 0.5, 0), m_CollisionAt);
			sphereBullet->GetTransform().SetPosition(Vector3(position.x, position.y + 0.5, position.z) + (direction * 5));
	}
	else
	{
		sphereBullet->GetTransform().SetPosition(m_World->GetMainCamera()->GetPosition() + (direction * 5));
	}
	Vector3 forceInDirection = direction * 10.0f;
	sphereBullet->GetPhysicsObject()->AddForce(forceInDirection);
}

Vector3 NCL::CSC8503::PaintBallClass::GetDirection(Vector3 fromPosition, Vector3 toPosition)
{
	return (toPosition - fromPosition).Normalised();
}

void NCL::CSC8503::PaintBallClass::UpdateTargetObject(GameObject* targetObject, GameWorld* world, Vector3 collisionAt)
{
	m_TargetObjet = targetObject;
	m_World = world;
	m_CollisionAt = collisionAt;
}
