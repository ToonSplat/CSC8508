#pragma once

#include "GameTechRenderer.h"

#include "ToonLevelManager.h"
#include <reactphysics3d/reactphysics3d.h>
#include "PaintBallClass.h"
#include "ToonEventListener.h"
#include "PushdownState.h"
#include "PlayerControl.h"

namespace NCL
{
	namespace CSC8503
	{
		class ToonFollowCamera;
		class ToonMinimapCamera;

		class ToonMapCamera;
		class ToonGame : public PushdownState
		{
		public:
			ToonGame(GameTechRenderer* renderer, bool offline = true);
			~ToonGame();

			virtual void UpdateGame(float dt);

		protected:
			virtual void StartGame();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void ShowUI(float time);
			Team* DetermineWinner(std::map<int, float> teamScores);

		protected:
			ToonFollowCamera* followCamera;
			Player*	player = nullptr;
			PlayerControl* playerControl = nullptr;

			GameTechRenderer* renderer;
			ToonGameWorld* world;
			ToonLevelManager* levelManager;
			PaintBallClass* baseWeapon;
			std::unordered_set<Player*> allPlayers;

			Team* tieTeam;
			Team* winner;
			float gameTime;
			bool closeGame = false;
			bool offline;
			const double timeStep = 1.0 / 60.0;
			double accumulator;
		};
	}
}