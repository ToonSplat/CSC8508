#pragma once
#include "NetworkBase.h"
#include "ToonObjectState.h"
#include "PlayerControl.h"

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
		short	pos[3];
		short	orientation[4];
		short	linVel[3];
		short	angVel[3];

		DeltaPacket() {
			type = Delta_State;
			size = sizeof(DeltaPacket) - sizeof(GamePacket);
		}
	};

	struct ClientPacket : public GamePacket {
		int		lastID;
		int		playerID;
		PlayerControl controls;

		ClientPacket() {
			type = Client_Update;
			size = sizeof(ClientPacket) - sizeof(GamePacket);
		}
	};
}