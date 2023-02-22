#pragma once
struct PlayerControl {
	short	direction[2]; // X, Z
	short	camera[2]; // Pitch, Yaw
	bool	aiming;
	bool	shooting;
	bool	jumping;

	PlayerControl() {
		direction[0]	= 0;
		direction[1]	= 0;
		camera[0]		= 0;
		camera[1]		= 0;
		aiming			= false;
		shooting		= false;
		jumping			= false;
	}
};

#include "Window.h"
#include "Camera.h"

using namespace NCL;

static void UpdateControlsByKeyboard(PlayerControl* controls, Camera* camera) {
	Maths::Vector3 forward = camera->GetForward();
	Maths::Vector3 right = camera->GetRight();
	Maths::Vector3 up = camera->GetUp();

	Maths::Vector3 linearMovement;
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::W))
	{
		linearMovement += forward;
	}
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::S))
	{
		linearMovement -= forward;
	}

	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::A))
	{
		linearMovement -= right;
	}
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::D))
	{
		linearMovement += right;
	}

	controls->direction[0] = short(linearMovement.x * 10000);
	controls->direction[1] = short(linearMovement.z * 10000);

	controls->camera[0] = (short)camera->GetPitch();
	controls->camera[1] = (short)camera->GetYaw();

	controls->aiming = Window::GetMouse()->ButtonHeld(MouseButtons::RIGHT);
	controls->shooting = controls->shooting || Window::GetMouse()->ButtonPressed(MouseButtons::LEFT);
	controls->jumping = controls->jumping || Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE);
}