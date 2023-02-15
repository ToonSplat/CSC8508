#pragma once
#include "ToonGame.h"
#include "NetworkBase.h"
#include <map>

namespace NCL {

	namespace CSC8503 {
		class GameServer;
		class GameClient;
		class NetworkPlayer;

		

		class ToonNetworkedGame : public ToonGame, public PacketReceiver {
		public:
			ToonNetworkedGame(GameTechRenderer* renderer);
			ToonNetworkedGame(GameTechRenderer* renderer, int a, int b, int c, int d);
			~ToonNetworkedGame();

			void StartAsServer();
			void StartAsClient(char a, char b, char c, char d);

			void UpdateGame(float dt) override;

			Player* SpawnPlayer(int playerID);

			void ServerStartLevel();
			void StartLevel();

			bool IsServer() { return thisServer; }
			bool IsClient() { return thisClient; }

			void ReceivePacket(int type, GamePacket* payload, int source) override;

		protected:
			void UpdateAsServer(float dt);
			void UpdateAsClient(float dt);

			void BroadcastSnapshot(bool deltaFrame);
			void UpdateMinimumState();

			GameServer* thisServer;
			GameClient* thisClient;
			float timeToNextPacket;
			int packetsToSnapshot;

			int myID;
			int winner;
			int myState;

			std::vector<ToonNetworkObject*> networkObjects;

			std::map<int, Player*> serverPlayers;
			std::map<int, PlayerControl*> playerControls;
			std::map<int, int> stateIDs;
			GameObject* localPlayer;
		};
	}
}

