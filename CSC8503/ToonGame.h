#pragma once

#include "GameTechRenderer.h"
#include "PhysicsSystem.h"

#include "ToonLevelManager.h"
#include <reactphysics3d/reactphysics3d.h>

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
			ToonGameWorld* world;
			ToonLevelManager* levelManager;

			reactphysics3d::PhysicsCommon physicsCommon;
			reactphysics3d::PhysicsWorld* physicsWorld;

			bool showCursor;
			const float timeStep = 1.0f / 60.0f;
			float accumulator;
		};
	}
}