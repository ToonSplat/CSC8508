#include "XboxControllerInput.h"
#include <Windows.h>
#include <Xinput.h>

using namespace NCL;

void XboxControllerInput::UpdateState() {
	UpdateLastState();
	XINPUT_STATE state;;
	
	DWORD result = XInputGetState(index, &state);
	if (result == ERROR_SUCCESS) {
		inputs.moveDir = GetStickMovement(state.Gamepad.sThumbLX, state.Gamepad.sThumbLY);
		inputs.relativeMousePosition = GetStickMovement(state.Gamepad.sThumbRX, state.Gamepad.sThumbRY) * CONTROLLER_SENSITIVITY;
		inputs.relativeMousePosition.y *= -1; // Default is inverted!
		inputs.aiming = (state.Gamepad.bLeftTrigger > 0);
		inputs.shooting = (state.Gamepad.bRightTrigger > 0);
		inputs.jumping = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X);
		inputs.selecting = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
		inputs.back = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
	}

	BaseInput::UpdateState();
}

Vector2 XboxControllerInput::GetStickMovement(float thumbX, float thumbY) {
	float magnitude = sqrt(thumbX * thumbX + thumbY * thumbY);
	float normalizedMagnitude = 0;

	// Determine how far the controller is pushed
	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		// Adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		// Normalize magnitude
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		if (normalizedMagnitude > 1.0f) {
			normalizedMagnitude = 1.0f;
		}

		float x = thumbX / 32767.0f * normalizedMagnitude;
		float y = thumbY / 32767.0f * normalizedMagnitude;
		return Vector2(x, y);
	}
	return Vector2(0, 0);
}