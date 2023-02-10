#pragma once
#include "NetworkBase.h"
#include "ToonObjectState.h"

namespace NCL::CSC8503 {

	struct FullPacket : public GamePacket {
		int		objectID = -1;
		ToonObjectState fullState;

		FullPacket() {
			type = Full_State;
			size = sizeof(FullPacket) - sizeof(GamePacket);
		}
	};

	struct DeltaPacket : public GamePacket {
		int		fullID = -1;
		int		objectID = -1;
		char	pos[3];
		char	orientation[4];
		//char	vel[3];
		//char	angVel[3];

		DeltaPacket() {
			type = Delta_State;
			size = sizeof(DeltaPacket) - sizeof(GamePacket);
		}
	};

	struct ClientPacket : public GamePacket {
		int		lastID;
		int		playerID;
		float	direction[3]; // X, Y, Z
		int		camera[2]; // Pitch, Yaw

		ClientPacket() {
			type = Client_Update;
			size = sizeof(ClientPacket) - sizeof(GamePacket);
		}
	};
}