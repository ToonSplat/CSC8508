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
			void UpdateCamera(float dt);
			void UpdateTesting();

		protected:
			ToonFollowCamera* followCamera;
			ToonMinimapCamera* minimapCamera;

			Player*	player = nullptr;
			ToonGameObject* targetObject = nullptr;
			PaintBallClass* sampleWeapon;

			PaintableZone* mainZone;
			std::vector<PaintableZone*>* subZones; // TODO: This can maybe be stored better.... only doing as vector for easy delete

			GameTechRenderer* renderer;
			ToonGameWorld* world;
			ToonLevelManager* levelManager;
			PaintBallClass* baseWeapon;
			Team* testTeam;

			bool offline;
			bool showCursor;
			const double timeStep = 1.0 / 60.0;
			double accumulator;
		};
	}
}