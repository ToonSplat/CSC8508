#include "BaseInput.h"

bool NCL::BaseInput::invert = false;

void NCL::BaseInput::UpdateState() {
	if (BaseInput::invert) {
		inputs.relativeMousePosition.y *= -1;
	}
}

void NCL::BaseInput::UpdateGameControls(PlayerControl* controls, Camera* camera) {
	Vector3 forward = camera->GetForward();
	Vector3 right = camera->GetRight();
	Vector3 up = camera->GetUp();

	Maths::Vector3 linearMovement;
	linearMovement += forward * inputs.moveDir.y;
	linearMovement += right * inputs.moveDir.x;

	controls->direction[0] = short(linearMovement.x * 1000);
	controls->direction[1] = short(linearMovement.z * 1000);

	controls->camera[0] = short(camera->GetPitch());
	controls->camera[1] = short(camera->GetYaw());

	controls->aiming = IsAiming();
	controls->shooting = IsShooting();
	controls->jumping = IsJumping();
}

void NCL::BaseInput::UpdateLastState() {
	if (inputs.moveDir.y > MINIMUM_MOVEMENT) inputs.lastUp = true; else inputs.lastUp = false;
	if (inputs.moveDir.y < -MINIMUM_MOVEMENT) inputs.lastDown = true; else inputs.lastDown = false;
	if (inputs.moveDir.x > MINIMUM_MOVEMENT) inputs.lastRight = true; else inputs.lastRight = false;
	if (inputs.moveDir.x < -MINIMUM_MOVEMENT) inputs.lastLeft = true; else inputs.lastLeft = false;
	inputs.mousePosition = Vector2(0, 0);
	inputs.lastShooting = inputs.shooting;
	inputs.lastJumping = inputs.jumping;
	inputs.lastSelecting = inputs.selecting;
	inputs.lastBack = inputs.back;
}