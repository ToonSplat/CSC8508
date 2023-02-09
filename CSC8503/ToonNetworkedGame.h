#pragma once
#include "ToonGame.h"
#include "NetworkBase.h"
#include <map>

namespace NCL {

	namespace CSC8503 {
		class GameServer;
		class GameClient;
		class NetworkPlayer;

		struct PlayerControl { // TODO: Update to PS4 precise direction, change to 3f?
			char	buttonstates[4]; // UP, LEFT, RIGHT, DOWN
			int		camera[2]; // Pitch, Yaw

			PlayerControl() {
				buttonstates[0] = 0;
				buttonstates[1] = 0;
				buttonstates[2] = 0;
				buttonstates[3] = 0;
				camera[0] = 0;
				camera[1] = 0;
			}
		};

		class ToonNetworkedGame : public ToonGame, public PacketReceiver {
		public:
			ToonNetworkedGame();
			ToonNetworkedGame(int a, int b, int c, int d);
			~ToonNetworkedGame();

			void StartAsServer();
			void StartAsClient(char a, char b, char c, char d);

			void UpdateGame(float dt) override;

			Player* SpawnPlayer(int playerID);

			void ServerStartLevel();
			void StartLevel();

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

			std::vector<NetworkObject*> networkObjects;

			std::map<int, Player*> serverPlayers;
			std::map<int, PlayerControl*> playerControls;
			std::map<int, int> stateIDs;
			GameObject* localPlayer;
		};
	}
}

