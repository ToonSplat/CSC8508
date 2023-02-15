#pragma once
#include "Vector3.h"

namespace NCL {
	namespace CSC8503 {
		using namespace std;
		using namespace NCL::Maths;
		class Team {
		public:
			Team(string name, Vector3 colour) : teamName(name), teamColour(colour) { percentageOwned = 0; }
			Team(string name, Vector3 colour) : teamName(name), teamColour(colour) {
				playerCount = 0;
			}
			~Team(void) = default;

			void AddPlayer(void) { playerCount++; }
			void RemovePlayer(void) { playerCount--; }
			int getPlayerCount(void) const { return playerCount; }
			string getTeamName(void) const { return teamName; }
			Vector3 getTeamColour(void) const { return teamColour; }

			void SetPercentageOwned(float newPercentage) { percentageOwned = newPercentage; }
			float GetPercentageOwned() const { return percentageOwned; }
		private:
			int playerCount;
			string teamName;
			Vector3 teamColour;
			float percentageOwned;
		};
	}
}