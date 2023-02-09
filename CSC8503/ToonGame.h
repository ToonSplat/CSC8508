#pragma once

#include "GameTechRenderer.h"
#include "PhysicsSystem.h"

#include "ToonLevelManager.h"
#include <reactphysics3d/reactphysics3d.h>
#include "PaintBallClass.h"
#include "ToonEventListener.h"

namespace NCL
{
	namespace CSC8503
	{
		class ToonFollowCamera;
		class ToonMinimapCamera;
		class ToonGame
		{
		public:
			ToonGame(bool offline = true);
			~ToonGame();

			virtual void UpdateGame(float dt);

		protected:
			Player*	player = nullptr;

			GameTechRenderer* renderer;
			ToonGameWorld* world;
			ToonLevelManager* levelManager;
			PaintBallClass* baseWeapon;
			Team* testTeam;

			bool offline;
			bool showCursor;
			const float timeStep = 1.0f / 60.0f;
			float accumulator;
		};
	}
}