#pragma once
struct PlayerControl {
	short	direction[3]; // X, Y, Z
	short	camera[2]; // Pitch, Yaw
	short	animDir[2];	//Keys WASD keys / X and Z
	bool	aiming;
	bool	shooting;
	bool	jumping;

	PlayerControl() {
		direction[0]	= 0;
		direction[1]	= 0;
		direction[2]	= 0;
		camera[0]		= 0;
		camera[1]		= 0;
		animDir[0]		= 0;
		animDir[1]		= 0;
		aiming			= false;
		shooting		= false;
		jumping			= false;
	}
};