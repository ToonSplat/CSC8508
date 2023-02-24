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
	InputType	input;

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