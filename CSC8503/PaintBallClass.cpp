#include "PaintBallClass.h"
#include "RenderObject.h"
#include "PhysicsObject.h"
#include "PaintBallProjectile.h"
#include "ToonLevelManager.h"
#include "ToonRenderObject.h"

using namespace NCL;
using namespace CSC8503;

PaintBallClass::PaintBallClass(){}

PaintBallClass::PaintBallClass(int _maxAmmoInUse, int _maxAmmoHeld, float _fireRate, float _reloadTime, float _maxShootDist,
	ShaderBase* basicShader, MeshGeometry* sphereMesh) :
	ammoInUse(_maxAmmoInUse), 
	ammoHeld(_maxAmmoHeld), 
	maxAmmoInUse(_maxAmmoInUse), 
	maxAmmoHeld(_maxAmmoHeld),
	fireRate(_fireRate), 
	reloadTime(_reloadTime), 
	maxShootDistance(_maxShootDist), 
	m_BasicShader(basicShader), 
	m_SphereMesh(sphereMesh) 
{
	owningObject = nullptr;
	team = nullptr;

	shootTimer = 0.0f;
	reloadTimer = 0.0f;
	status = isIdle;
	for (int i = 0; i < trajectoryPoints; i++)
	{
		bullet[i] = nullptr;
	}
}

PaintBallClass::~PaintBallClass() 
{
}

PaintBallClass PaintBallClass::MakeInstance() {
	return PaintBallClass(maxAmmoInUse, maxAmmoHeld, fireRate, reloadTime, maxShootDistance, m_BasicShader, m_SphereMesh);
}

float PaintBallClass::GetYCoordinate(int x, int initialVelocity)
{
	return (x * tan(ToonGameWorld::Get()->GetMainCamera()->GetPitch()) - ((9.8 * x * x) / (2 * initialVelocity * initialVelocity * cos(ToonGameWorld::Get()->GetMainCamera()->GetPitch()))));
}

void PaintBallClass::Update(float dt) {
	//if left mouse status
	//if (ammoInUse == 0) std::cout << "WEAPON LOAD FAIL?\n";

	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::LEFT) && ammoInUse > 0)
		status = isFiring;
	else if (ammoInUse <= 0 || Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::R))
		status = isReloading;
	else
		status = isIdle;

	switch (status) 
	{
		case isIdle:
			break;
		case isFiring:
			Shoot(dt);
			break;
		case isReloading:
			Reload(dt);
			break;
	}


	if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::RIGHT) && isIdle)
	{
		DrawTrajectory(10);
	}
	else
	{
		HideTrajectory();
	}
}

void PaintBallClass::DrawTrajectory(float force)
{
	reactphysics3d::Vector3 orientation = owningObject->GetRigidbody()->getTransform().getOrientation() * reactphysics3d::Quaternion::fromEulerAngles(reactphysics3d::Vector3((ToonGameWorld::Get()->GetMainCamera()->GetPitch() + 10) / 180.0f * _Pi, 0, 0)) * reactphysics3d::Vector3(0, 0, -10.0f);
	orientation.normalize();
	reactphysics3d::Vector3 position	= owningObject->GetRigidbody()->getTransform().getPosition() + orientation * 5 + reactphysics3d::Vector3(0, 1, 0);
	reactphysics3d::Vector3 forceVector = orientation * force;
	reactphysics3d::Vector3 velocity	= forceVector;//(forceVector / 0.1) * 0.1;
	float flightDurartion				= (2 * velocity.y) / 9.8;
	float singlePointTime				= flightDurartion / trajectoryPoints;

	for (int i = 0; i < trajectoryPoints; i++)
	{
		float deltaTime = singlePointTime * i;
		float x			= velocity.x * deltaTime;
		float y			= velocity.y * deltaTime - (0.5f * 9.8 * deltaTime * deltaTime);
		float z		    = velocity.z * deltaTime;
		position.x	   += x;
		position.y	   += y;
		position.z	   += z;

		if (!bullet[i])
		{
			bullet[i] = new PaintBallProjectile(ToonGameWorld::Get()->GetPhysicsWorld(), position, orientation, 0.1, 0.1f, team);
			bullet[i]->GetRigidbody()->setIsActive(false);
		}
		bullet[i]->SetPosition(position.x, position.y, position.z);
		bullet[i]->GetRigidbody()->setMass(0.1);
	}
}

void PaintBallClass::HideTrajectory()
{
	for (int i = 0; i < trajectoryPoints; i++)
	{
		if (bullet[i])
		{
			bullet[i]->SetPosition(Vector3(-100, -100, -100));
		}
	}
}

void PaintBallClass::Shoot(float dt) {
	shootTimer += dt;
	if (/*shootTimer >= fireRate &&*/ ammoInUse > 0) 
	{
		// Shoot Projectile here
		FireBullet();
		//std::cout << "Weapon is shooting" << std::endl;
		ammoInUse--;
		shootTimer = 0.0f;
	}
}

void PaintBallClass::Reload(float dt) 
{
	if (ammoInUse < maxAmmoInUse && ammoHeld > 0) 
	{
		reloadTimer += dt;
		if (reloadTimer >= reloadTime) 
		{
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

void PaintBallClass::FireBullet() 
{
	if (owningObject == nullptr)
		return;

	reactphysics3d::Vector3 orientation = owningObject->GetRigidbody()->getTransform().getOrientation() * reactphysics3d::Quaternion::fromEulerAngles(reactphysics3d::Vector3((ToonGameWorld::Get()->GetMainCamera()->GetPitch() + 10) / 180.0f * _Pi, 0, 0)) * reactphysics3d::Vector3(0, 0, -10.0f); // TODO: Update this to Sunit's new method of getting angle
	orientation.normalize();
	reactphysics3d::Vector3 position = owningObject->GetRigidbody()->getTransform().getPosition() + orientation * 5 + reactphysics3d::Vector3(0, 1, 0);

	PaintBallProjectile* bullet = new PaintBallProjectile(ToonGameWorld::Get()->GetPhysicsWorld(), position, orientation, 0.25f, 2.5f, team);
	bullet->GetRigidbody()->applyWorldForceAtCenterOfMass(orientation * 250.0f); // TODO: The force can maybe be applied better

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