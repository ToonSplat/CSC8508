#pragma once
struct PlayerControl {
	enum InputType {
		Keyboard	= 0,
		Xbox		= 1,
		PlayStation	= 2
	};
	short	direction[2]; // X, Z
	short	camera[2]; // Pitch, Yaw
	bool	aiming;
	bool	shooting;
	bool	jumping;
	bool	shootTriggerReleased;
	InputType	input;

	PlayerControl() {
		direction[0]	= 0;
		direction[1]	= 0;
		camera[0]		= 0;
		camera[1]		= 0;
		aiming			= false;
		shooting		= false;
		jumping			= false;
		shootTriggerReleased	= false;
	}
};

#include "Window.h"
//#include <Xinput.h>
#include "Camera.h"

using namespace NCL;

extern void UpdateControls(PlayerControl* controls, Camera* camera);

extern void UpdateCamera(PlayerControl* controls, Camera* camera);

extern void UpdateControlsByKeyboard(PlayerControl* controls, Camera* camera);

//static void UpdateControlsByXboxController(PlayerControl* controls, Camera* camera, int index = 0);

//static bool IsXboxButtonPressed(XINPUT_STATE& state, int button);

//static bool IsXboxTriggerPressed(BYTE trigger);