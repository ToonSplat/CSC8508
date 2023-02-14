#pragma once

#include "GameTechRenderer.h"

#include "ToonLevelManager.h"
#include <reactphysics3d/reactphysics3d.h>
#include "PaintBallClass.h"
#include "PlayerControl.h"

namespace NCL
{
	namespace CSC8503
	{
		class ToonFollowCamera;
		class ToonMinimapCamera;
		class ToonGame
		{
		public:
			ToonGame(GameTechRenderer* renderer, bool offline = true);
			~ToonGame();

			virtual void UpdateGame(float dt);

		protected:
			void UpdateControls(PlayerControl* controls);
			Player*	player = nullptr;
			PlayerControl* playerControl = nullptr;

			GameTechRenderer* renderer;
			ToonGameWorld* world;
			ToonLevelManager* levelManager;
			PaintBallClass* baseWeapon;

			bool offline;
			bool showCursor;
			const double timeStep = 1.0 / 60.0;
			double accumulator;
		};
	}
}