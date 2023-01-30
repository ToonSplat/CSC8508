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

			PaintableZone* mainZone;
			std::vector<PaintableZone*>* subZones; // TODO: This can maybe be stored better.... only doing as vector for easy delete

			bool showCursor;
		};
	}
}