#pragma once
struct PlayerControl {
	float	direction[3]; // X, Y, Z
	int		camera[2]; // Pitch, Yaw
	bool	aiming;
	bool	shooting;
	bool	jumping;

	PlayerControl() {
		direction[0]	= 0;
		direction[1]	= 0;
		direction[2]	= 0;
		camera[0]		= 0;
		camera[1]		= 0;
		aiming			= false;
		shooting		= false;
		jumping			= false;
	}
};