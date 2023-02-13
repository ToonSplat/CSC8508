#pragma once
#include "NetworkBase.h"
#include "ToonNetworkPackets.h"
#include "ToonObjectState.h"
#include <vector>
namespace NCL {
	namespace CSC8503 {
		class ToonGameObject;
		class ToonNetworkObject {
		public:
			ToonNetworkObject(ToonGameObject* o, int id, int startState = 0);
			virtual ~ToonNetworkObject();

			//Called by clients
			virtual bool ReadPacket(GamePacket& p);
			//Called by servers
			virtual bool WritePacket(GamePacket** p, bool deltaFrame, int stateID);

			void SetGameObject(ToonGameObject* newObject) { object = newObject; }

			void UpdateStateHistory(int minID);

			int GetNetworkID() const { return networkID; }

		protected:

			ToonObjectState& GetLatestNetworkState();

			void UpdateLastFullState();

			bool GetNetworkState(int frameID, ToonObjectState& state);

			virtual bool ReadDeltaPacket(DeltaPacket& p);
			virtual bool ReadFullPacket(FullPacket& p);

			virtual bool WriteDeltaPacket(GamePacket** p, int stateID);
			virtual bool WriteFullPacket(GamePacket** p);

			ToonGameObject* object;

			ToonObjectState lastFullState;

			std::vector<ToonObjectState> stateHistory;

			int networkID;
		};
	}
}