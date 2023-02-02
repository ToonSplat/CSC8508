#include "PaintBallClass.h"
#include "RenderObject.h"
#include "PhysicsObject.h"
#include "ToonLevelManager.h"
#include "ToonRenderObject.h"

using namespace NCL;
using namespace CSC8503;

PaintBallClass::PaintBallClass(){}

PaintBallClass::PaintBallClass(int _maxAmmoInUse, int _maxAmmoHeld, int _fireRate, int _reloadTime, float _maxShootDist,
	ShaderBase* basicShader, MeshGeometry* sphereMesh) :
	ammoInUse(_maxAmmoInUse), ammoHeld(_maxAmmoHeld), maxAmmoInUse(_maxAmmoInUse), maxAmmoHeld(_maxAmmoHeld),
	fireRate(_fireRate), reloadTime(_reloadTime), maxShootDistance(_maxShootDist), m_BasicShader(basicShader), m_SphereMesh(sphereMesh) {

	shootTimer = 0.0f;
	reloadTimer = 0.0f;
}

PaintBallClass::~PaintBallClass() {
	
}

PaintBallClass PaintBallClass::MakeInstance() {
	return PaintBallClass(maxAmmoInUse, maxAmmoHeld, fireRate, reloadTime, maxShootDistance, m_BasicShader, m_SphereMesh);
}

void PaintBallClass::Update(float dt) {
	//if left mouse status
	if (ammoInUse == 0) std::cout << "WEAPON LOAD FAIL?\n";
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

void PaintBallClass::CreateBullet() {
	std::cout << "BANG" << std::endl;
	ToonGameObject* bullet = ToonLevelManager::Get()->MakeBullet();
	bullet->GetRenderObject()->SetColour(Vector4(0.0f, 1.0f, 1.0f, 1.0f));
	reactphysics3d::Vector3 orientation = owningObject->GetRigidbody()->getTransform().getOrientation() * reactphysics3d::Quaternion::fromEulerAngles(reactphysics3d::Vector3((ToonGameWorld::Get()->GetMainCamera()->GetPitch() +10) / 180.0f * _Pi, 0, 0)) * reactphysics3d::Vector3(0, 0, -10.0f); // TODO: Update this to Sunit's new method of getting angle
	bullet->SetOrientation(orientation);
	orientation.normalize();
	bullet->SetPosition(owningObject->GetRigidbody()->getTransform().getPosition() + orientation * 5 + reactphysics3d::Vector3(0, 1, 0));
	std::cout << "Goat Position: " << owningObject->GetRigidbody()->getTransform().getPosition().to_string() <<
		"\nGoat and Bullet Orientation: " << orientation.to_string() <<
		"\nBulletPosition: " << bullet->GetRigidbody()->getTransform().getPosition().to_string() << std::endl;
	bullet->GetRigidbody()->applyWorldForceAtCenterOfMass(orientation * 250.0f); // TODO: The force can maybe be applied better
	/*ToonLevelManager::Get();*/
}

//void PaintBallClass::CreateBullet()
//{
//	GameObject*   sphereBullet = new GameObject("Fire");
//	float		  radius	   = 0.1f;
//	Vector3		  sphereSize   = Vector3(radius, radius, radius);
//	SphereVolume* volume	   = new SphereVolume(radius);
//	sphereBullet->SetBoundingVolume((CollisionVolume*)volume);
//
//	sphereBullet->GetTransform().SetScale(sphereSize);
//	sphereBullet->SetRenderObject(new RenderObject(&sphereBullet->GetTransform(), m_SphereMesh, nullptr, m_BasicShader));
//	sphereBullet->SetPhysicsObject(new PhysicsObject(&sphereBullet->GetTransform(), sphereBullet->GetBoundingVolume()));
//
//	sphereBullet->GetPhysicsObject()->SetInverseMass(100.0f);
//	sphereBullet->GetRenderObject()->SetColour(Vector4(0.0f, 1.0f, 1.0f, 1.0f));
//	sphereBullet->GetPhysicsObject()->InitSphereInertia();
//	//ToonGameWorld::Get()->AddGameObject(sphereBullet);
//
//
//	Vector3 position = owningObject->GetTransform().GetPosition();
//	Vector3 direction = (Matrix4::Rotation(ToonGameWorld::Get()->GetMainCamera()->GetYaw(), Vector3(0, 1, 0)) * Matrix4::Rotation(ToonGameWorld::Get()->GetMainCamera()->GetPitch(), Vector3(1, 0, 0)) * Vector3(0, 0, -1)) + Vector3(0, 0.05f, 0);
//	sphereBullet->GetTransform().SetPosition(Vector3(position.x, position.y, position.z) + (direction * 5));
//
//	//Vector3 direction = CollisionDetection::BuildRayFromCenter(*ToonGameWorld::Get()->GetMainCamera()).GetDirection() + Vector3(0, 0.03f, 0);
//	Vector3 forceInDirection = direction * 100.0f;
//	sphereBullet->GetPhysicsObject()->AddForce(forceInDirection);
//}

void NCL::CSC8503::PaintBallClass::UpdateTargetObject(ToonGameObject* targetObject)
{
	m_TargetObjet = targetObject;
}
