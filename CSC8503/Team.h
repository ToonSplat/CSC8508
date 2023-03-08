#pragma once
#include "Vector3.h"
#include <random>

namespace NCL {
	namespace CSC8503 {
		using namespace std;
		using namespace NCL::Maths;

		struct TeamSpawnPointData
		{
			TeamSpawnPointData(const Vector3 _pos, const Vector3 _rot) : m_pos(_pos), m_rot(_rot) {}
			Vector3 GetPosition() const { return m_pos; }
			Vector3 GetRotation() const { return m_rot; }

		private:
			Vector3 m_pos;
			Vector3 m_rot;
		};		

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

			static const Vector3 T_GREEN_GOBLINS;
			static const Vector3 T_PURPLE_PRAWNS;
			static const Vector3 T_BLUE_BULLDOGS;
			static const Vector3 T_ORANGE_OTTERS;

			void AddSpawnPoint(const TeamSpawnPointData& data) 
			{
				std::cout << "Added Spawn Point for Team ID: " << teamID << std::endl;
				spawnPointsData.emplace_back(data); 
			}
			const TeamSpawnPointData GetRandomSpawnPoint()
			{
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> dis(0, (int)spawnPointsData.size() - 1);

				int randomIndex = dis(gen);
				return spawnPointsData.at(randomIndex);
			}

		private:
			int teamID;
			int playerCount;
			string teamName;
			Vector3 teamColour;
			float percentageOwned;
			std::vector<TeamSpawnPointData> spawnPointsData;
		};
	}
}