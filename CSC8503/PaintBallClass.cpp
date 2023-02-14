#include "PaintBallClass.h"
#include "RenderObject.h"
#include "PhysicsObject.h"
#include "PaintBallProjectile.h"
#include "ToonLevelManager.h"
#include "ToonRenderObject.h"
#include "ToonUtils.h"
#include "ToonRaycastCallback.h"
#include "ToonGameWorld.h"
#include "ToonLevelManager.h"

using namespace NCL;
using namespace CSC8503;

PaintBallClass::PaintBallClass(){}

PaintBallClass::PaintBallClass(ToonGameWorld* gameWorld, ToonLevelManager* levelManager, int _maxAmmoInUse, int _maxAmmoHeld, float _fireRate, float _reloadTime, float _maxShootDist) :
	gameWorld(gameWorld),
	levelManager(levelManager),
	ammoInUse(_maxAmmoInUse), 
	ammoHeld(_maxAmmoHeld), 
	maxAmmoInUse(_maxAmmoInUse), 
	maxAmmoHeld(_maxAmmoHeld),
	fireRate(_fireRate), 
	reloadTime(_reloadTime), 
	maxShootDistance(_maxShootDist)
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

	trajectoryDetected = false;
}

PaintBallClass::~PaintBallClass() 
{
}

PaintBallClass PaintBallClass::MakeInstance() {
	return PaintBallClass(gameWorld, levelManager, maxAmmoInUse, maxAmmoHeld, fireRate, reloadTime, maxShootDistance);
}

float PaintBallClass::GetYCoordinate(int x, int initialVelocity)
{
	return (x * tan(gameWorld->GetMainCamera()->GetPitch()) - ((9.8 * x * x) / (2 * initialVelocity * initialVelocity * cos(gameWorld->GetMainCamera()->GetPitch()))));
}

NCL::Maths::Vector3 NCL::CSC8503::PaintBallClass::CalculateBulletVelocity(NCL::Maths::Vector3 target, NCL::Maths::Vector3 origin, float t)
{
	Vector3 distance = target - origin;
	Vector3 distanceXZ = distance;
	distanceXZ.y = 0.0f;

	float sY = distance.y;
	float sXZ = distanceXZ.Length();

	float VxZ = sXZ * t;
	float Vy = (sY / t) + (0.5f * abs(gameWorld->GetPhysicsWorld().getGravity().y) * t);

	Vector3 result = distanceXZ.Normalised();
	result *= VxZ;
	result.y = Vy;

	return result;
;}

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


	if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::RIGHT))
	{
		Vector3 playerPos = ToonUtils::ConvertToNCLVector3(owningObject->GetRigidbody()->getTransform().getPosition());

		reactphysics3d::Quaternion pRot = owningObject->GetRigidbody()->getTransform().getOrientation();
		Quaternion nRot(pRot.x, pRot.y, pRot.z, pRot.w);
		Matrix4 owningMat = Matrix4(nRot);

		Vector3 forward = gameWorld->GetMainCamera()->GetForward();
		Vector3 startRay = gameWorld->GetMainCamera()->GetPosition();
		Vector3 endRay = startRay + forward * 500.0f;

		reactphysics3d::Ray shootRay(ToonUtils::ConvertToRP3DVector3(startRay), ToonUtils::ConvertToRP3DVector3(endRay));
		ToonRaycastCallback shootRayCallback;
		gameWorld->GetPhysicsWorld().raycast(shootRay, &shootRayCallback, ToonCollisionLayer::Default);

		trajectoryDetected = shootRayCallback.IsHit();
		if (trajectoryDetected)
		{
			bulletVelocity = CalculateBulletVelocity(shootRayCallback.GetHitWorldPos(), startRay, 1.0f);
			//Debug::DrawLine(startRay, shootRayCallback.GetHitWorldPos(), Debug::BLUE, 1.0f);
		}

		DrawTrajectory(bulletVelocity);	//250->10
	}
	else
	{
		HideTrajectory();
	}

	//if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::RIGHT))
	//{
	//	Vector3 playerPos = ToonUtils::ConvertToNCLVector3(owningObject->GetRigidbody()->getTransform().getPosition());

	//	reactphysics3d::Quaternion pRot = owningObject->GetRigidbody()->getTransform().getOrientation();
	//	Quaternion nRot(pRot.x, pRot.y, pRot.z, pRot.w);
	//	Matrix4 owningMat = Matrix4(nRot);

	//	Vector3 forward = gameWorld->GetMainCamera()->GetForward();
	//	Vector3 startRay = gameWorld->GetMainCamera()->GetPosition();
	//	Vector3 endRay = startRay + forward * 500.0f;
	//	
	//	reactphysics3d::Ray shootRay(ToonUtils::ConvertToRP3DVector3(startRay), ToonUtils::ConvertToRP3DVector3(endRay));
	//	ToonRaycastCallback shootRayCallback;
	//	gameWorld->GetPhysicsWorld().raycast(shootRay, &shootRayCallback, ToonCollisionLayer::Default);

	//	trajectoryDetected = shootRayCallback.IsHit();
	//	if (trajectoryDetected)
	//	{
	//		bulletVelocity = CalculateBulletVelocity(shootRayCallback.GetHitWorldPos(), startRay, 1.0f);
	//		//Debug::DrawLine(startRay, shootRayCallback.GetHitWorldPos(), Debug::BLUE, 1.0f);
	//	}
	//}
}

void PaintBallClass::DrawTrajectory(NCL::Maths::Vector3 force)
{
	const float PAINTBALL_RADIUS = 0.25f;
	const float PAINTBALL_IMPACT_RADIUS = 2.5f;
	reactphysics3d::Vector3 orientation = owningObject->GetRigidbody()->getTransform().getOrientation() * reactphysics3d::Quaternion::fromEulerAngles(reactphysics3d::Vector3((gameWorld->GetMainCamera()->GetPitch() + 10) / 180.0f * _Pi, 0, 0)) * reactphysics3d::Vector3(0, 0, -10.0f);
	orientation.normalize();
	reactphysics3d::Vector3 position	= owningObject->GetRigidbody()->getTransform().getPosition() + orientation * 3 + reactphysics3d::Vector3(0, 1, 0);
	//reactphysics3d::Vector3 forceVector = orientation * force;
	reactphysics3d::Vector3 velocity	= ToonUtils::ConvertToRP3DVector3(force);
	float flightDurartion				= (velocity.y) / -gameWorld->GetPhysicsWorld().getGravity().y;
	float singlePointTime				= flightDurartion / trajectoryPoints;

	if (flightDurartion < 0.0f) { HideTrajectory(); return; }
	for (int i = 0; i < trajectoryPoints; i++)
	{
		float deltaTime = singlePointTime * i;
		float x			= velocity.x * deltaTime;
		float y			= velocity.y * deltaTime - (0.5f * -gameWorld->GetPhysicsWorld().getGravity().y * deltaTime * deltaTime);
		float z		    = velocity.z * deltaTime;
		position.x	   += x;
		position.y	   += y;
		position.z	   += z;

		if (!bullet[i])
		{
			bullet[i] = levelManager->AddPaintBallProjectileToWorld(position, orientation, PAINTBALL_RADIUS, PAINTBALL_IMPACT_RADIUS, team);
			bullet[i]->GetRigidbody()->setIsActive(false);
		}
		bullet[i]->SetPosition(position.x, position.y, position.z);
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
	const float PAINTBALL_RADIUS = 0.25f;
	const float PAINTBALL_IMPACT_RADIUS = 2.5f;
	if (owningObject == nullptr)
		return;

	reactphysics3d::Vector3 orientation = owningObject->GetRigidbody()->getTransform().getOrientation() * reactphysics3d::Quaternion::fromEulerAngles(reactphysics3d::Vector3((gameWorld->GetMainCamera()->GetPitch() + 10) / 180.0f * _Pi, 0, 0)) * reactphysics3d::Vector3(0, 0, -10.0f); // TODO: Update this to Sunit's new method of getting angle
	orientation.normalize();
	reactphysics3d::Vector3 position = owningObject->GetRigidbody()->getTransform().getPosition() + orientation * 3 + reactphysics3d::Vector3(0, 0, 0);

	PaintBallProjectile* bullet = levelManager->AddPaintBallProjectileToWorld(position, orientation, PAINTBALL_RADIUS, PAINTBALL_IMPACT_RADIUS, team);
	//bullet->GetRigidbody()->setLinearVelocity(ToonUtils::ConvertToRP3DVector3(bulletVelocity));
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
//	//gameWorld->AddGameObject(sphereBullet);
//
//
//	Vector3 position = owningObject->GetTransform().GetPosition();
//	Vector3 direction = (Matrix4::Rotation(gameWorld->GetMainCamera()->GetYaw(), Vector3(0, 1, 0)) * Matrix4::Rotation(gameWorld->GetMainCamera()->GetPitch(), Vector3(1, 0, 0)) * Vector3(0, 0, -1)) + Vector3(0, 0.05f, 0);
//	sphereBullet->GetTransform().SetPosition(Vector3(position.x, position.y, position.z) + (direction * 5));
//
//	//Vector3 direction = CollisionDetection::BuildRayFromCenter(*gameWorld->GetMainCamera()).GetDirection() + Vector3(0, 0.03f, 0);
//	Vector3 forceInDirection = direction * 100.0f;
//	sphereBullet->GetPhysicsObject()->AddForce(forceInDirection);
//}