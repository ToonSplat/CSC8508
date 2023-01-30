#pragma once

#include "GameTechRenderer.h"
#include "PhysicsSystem.h"

#include "ToonLevelManager.h"

namespace NCL
{
	namespace CSC8503
	{
		class ToonGame
		{
		public:
			ToonGame();
			~ToonGame();

			virtual void UpdateGame(float dt);

		protected:
			GameTechRenderer* renderer;
			PhysicsSystem* physics;
			GameWorld* world;
			ToonLevelManager* levelManager;

			bool showCursor;
		};
	}
}