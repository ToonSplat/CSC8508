#pragma once
#include "Vector3.h"

namespace NCL {
	namespace CSC8503 {
		using namespace std;
		using namespace NCL::Maths;
		class Team {
		public:
			Team(string name, Vector3 colour, int ID) : teamName(name), teamColour(colour), teamID(ID) {
				playerCount = 0;
				percentageOwned = 0;
			}
			~Team(void) = default;

			int GetTeamID(void) const { return teamID; }
			void AddPlayer(void) { playerCount++; }
			void RemovePlayer(void) { playerCount--; }
			int GetPlayerCount(void) const { return playerCount; }
			string GetTeamName(void) const { return teamName; }
			Vector3 GetTeamColour(void) const { return teamColour; }

			void SetPercentageOwned(float newPercentage) { percentageOwned = newPercentage; }
			float GetPercentageOwned() const { return percentageOwned; }
		private:
			int teamID;
			int playerCount;
			string teamName;
			Vector3 teamColour;
			float percentageOwned;
		};
	}
}