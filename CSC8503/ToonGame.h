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

			bool offline;
			bool showCursor;
			const double timeStep = 1.0 / 60.0;
			double accumulator;
		};
	}
}