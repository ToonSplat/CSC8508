#include "PaintBallClass.h"

using namespace NCL;
using namespace CSC8503;

PaintBallClass::PaintBallClass(int _maxAmmoInUse, int _maxAmmoHeld, int _fireRate, int _reloadTime, float _maxShootDist) {
	ammoInUse = _maxAmmoInUse;
	ammoHeld = _maxAmmoHeld;
	maxAmmoHeld = _maxAmmoHeld;
	maxAmmoInUse = _maxAmmoInUse;
	fireRate = _fireRate;
	reloadTime = _reloadTime;
	maxShootDistance = _maxShootDist;

	shootTimer = 0.0f;
	reloadTimer = 0.0f;
}

PaintBallClass::~PaintBallClass() {

}

void PaintBallClass::Update(float dt) {
	//if left mouse status
	if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT) && ammoInUse > 0)
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