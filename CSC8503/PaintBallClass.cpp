#include "PaintBallClass.h"

using namespace NCL;
using namespace CSC8503;

PaintBallClass::PaintBallClass() {

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
		shootTimer = 0.0f;
	}
}

void PaintBallClass::Reload(float dt) {
	if (ammoInUse < maxAmmoInUse && ammoHeld > 0) {
		reloadTimer += dt;
		if (reloadTimer >= reloadTime) {
			//Reload Maths here
		}
	}
}

void PaintBallClass::PickUpAmmo() {

}