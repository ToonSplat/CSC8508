﻿#include "PlayerControl.h"

void UpdateControls(PlayerControl* controls, Camera* camera) {
	UpdateControlsByKeyboard(controls, camera); // Always keyboard for now
	UpdateCamera(controls, camera);
}

void UpdateCamera(PlayerControl* controls, Camera* camera) {
	controls->camera[0] = (short)camera->GetPitch();
	controls->camera[1] = (short)camera->GetYaw();
}

void UpdateControlsByKeyboard(PlayerControl* controls, Camera* camera) {
	//	##################################
	//	Movement
	//	##################################
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

	controls->direction[0] = short(linearMovement.x * 1000);
	controls->direction[1] = short(linearMovement.z * 1000);

	//	##################################
	//	Actions
	//	##################################

	controls->aiming = Window::GetMouse()->ButtonHeld(MouseButtons::RIGHT);
	controls->shooting = controls->shooting || Window::GetMouse()->ButtonPressed(MouseButtons::LEFT);
	controls->jumping = controls->jumping || Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE);
}

//// Index refers to index of the controller, from 1 to 4. Will probably map indices to players elsewhere
//static void UpdateControlsByXboxController(PlayerControl* controls, Camera* camera) {
//	Maths::Vector3 forward = camera->GetForward();
//	Maths::Vector3 right = camera->GetRight();
//	Maths::Vector3 up = camera->GetUp();
//
//	Maths::Vector3 linearMovement;
//
//	XINPUT_STATE state;
//	ZeroMemory(&state, sizeof(XINPUT_STATE));
//
//	DWORD result = XInputGetState(index, &state);
//	if (result == ERROR_SUCCESS) {
//		//	##################################
//		//	Movement
//		//	##################################
//		float thumbLX = state.Gamepad.sThumbLX;
//		float thumbLY = state.Gamepad.sThumbLY;
//		float magnitude = sqrt(thumbLX * thumbLX + thumbLY * thumbLY);
//		float normalizedMagnitude = 0;
//
//		// Determine how far the controller is pushed
//		if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
//			// Adjust magnitude relative to the end of the dead zone
//			magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
//
//			// Normalize magnitude
//			normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
//			if (normalizedMagnitude > 1.0f) {
//				normalizedMagnitude = 1.0f;
//			}
//
//			float x = thumbLX / 32767.0f * normalizedMagnitude;
//			linearMovement += right * x;
//			float y = thumbLY / 32767.0f * normalizedMagnitude;
//			linearMovement += forward * y;
//
//			controls->direction[0] = short(linearMovement.x * 1000);
//			controls->direction[1] = short(linearMovement.z * 1000);
//		}
//		//	##################################
//		//	Actions
//		//	###################################
//		controls->aiming = IsXboxTriggerPressed(state.Gamepad.bLeftTrigger);
//		controls->jumping = controls->jumping || IsXboxButtonPressed(state, XINPUT_GAMEPAD_A);
//		controls->shootTriggerReleased = !IsXboxTriggerPressed(state.Gamepad.bRightTrigger);
//		controls->shooting = controls->shooting || (IsXboxTriggerPressed(state.Gamepad.bRightTrigger) && controls->shootTriggerReleased);
//	}
//}
//
//
//static bool IsXboxButtonPressed(XINPUT_STATE& state, int button) {
//	return (state.Gamepad.wButtons & button) != 0;
//}
//
//static bool IsXboxTriggerPressed(BYTE trigger) {
//	return (trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
//}