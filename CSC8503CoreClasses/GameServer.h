#pragma once
#include "NetworkBase.h"

namespace NCL {
	namespace CSC8503 {
		class GameWorld;
		class GameServer : public NetworkBase {
		public:
			GameServer(int onPort, int maxClients);
			~GameServer();

			bool Initialise();
			void Shutdown();

			void SetGameWorld(GameWorld& g);

			bool SendGlobalPacket(int msgID);
			bool SendGlobalPacket(GamePacket& packet);
			bool SendPacketToClient(GamePacket& payload, int playerID);

			int GetPlayerCount() const { return clientCount; } 

			virtual void UpdateServer();

		protected:
			int			port;
			int			clientMax;
			int			clientCount;
			GameWorld* gameWorld;

			int incomingDataRate;
			int outgoingDataRate;
		};
	}
}
