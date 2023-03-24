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

		struct ImpactLocation {
			int location[3];
			char radius;
			char team;
			ImpactLocation(Vector3 location, float radius, int team) {
				this->location[0] = (int)(location.x * 100);
				this->location[1] = (int)(location.y * 100);
				this->location[2] = (int)(location.z * 100);
				this->radius = (char)(radius * 10);
				this->team = team;
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

			Player* SpawnPlayer(int playerID, Team* team, PlayerControl* controls = nullptr);

			void ServerStartGame();
			void StartGame() override;

			bool IsServer() { return thisServer; }
			bool IsClient() { return thisClient; }

			void ReceivePacket(int type, GamePacket* payload, int source) override;

			void AddHitsphereImpact(ToonGameObject* o, float radius, int teamID);
			void SendImpactPoint(ImpactPoint point, ToonGameObject* object, int playerID = -1);
			void SendHitsphereImpact(ImpactLocation location, int playerID = -1);

			void UpdateCall(float dt) override;
			PushdownState::PushdownResult DidSelectCancelButton() override;
			PushdownState::PushdownResult DidSelectOkButton() override;

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

			std::vector<ImpactLocation> impactLocations;
			std::vector<ToonNetworkObject*> networkObjects;

			std::map<int, PlayerDetails> serverPlayers;

			AudioEmitter* e;
      
			ToonConfirmationScreen* m_ConfirmationScreen;
			bool					m_ShouldShowConfirmationScreen = false;		//Just for showing confirmation screen
			bool					m_MoveBackOnConfirmation = false;		//For moving back if the player selects yes on confirmation
		};
	}
}
