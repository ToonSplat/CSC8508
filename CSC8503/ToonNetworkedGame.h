#pragma once
#include "ToonGame.h"
#include "NetworkBase.h"
#include <map>

namespace NCL {

	namespace CSC8503 {
		class GameServer;
		class GameClient;
		class NetworkPlayer;

		struct PlayerDetails {
			Player* player;
			PlayerControl* controls;
			Team* team;
			int StateID;

			PlayerDetails(Player* player, PlayerControl* controls, Team* team) {
				this->player = player;
				this->controls = controls;
				this->team = team;
				this->StateID = 0;
			}
		};

		class ToonNetworkedGame : public ToonGame, public PacketReceiver {
		public:
			ToonNetworkedGame(GameTechRenderer* renderer);
			ToonNetworkedGame(GameTechRenderer* renderer, int a, int b, int c, int d);
			~ToonNetworkedGame();

			void StartAsServer();
			void StartAsClient(char a, char b, char c, char d);

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;

			void UpdateGame(float dt) override;

			Player* SpawnPlayer(int playerID, Team* team);

			void ServerStartGame();
			void StartGame() override;

			bool IsServer() { return thisServer; }
			bool IsClient() { return thisClient; }

			void ReceivePacket(int type, GamePacket* payload, int source) override;

			void SendImpactPoint(ImpactPoint point, PaintableObject* object, int playerID = -1);

			void UpdateCall(float dt) override;

		protected:
			void UpdateAsServer(float dt);
			void UpdateAsClient(float dt);

			void BroadcastSnapshot(bool deltaFrame);
			void UpdateMinimumState();

			GameServer* thisServer;
			GameClient* thisClient;
			float timeToNextPacket;
			int packetsToSnapshot;

			float serverClosed = -256.0f;
			int myID;
			int myState;

			std::vector<ToonNetworkObject*> networkObjects;

			std::map<int, PlayerDetails> serverPlayers;
		};
	}
}

