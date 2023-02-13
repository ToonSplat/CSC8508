#pragma once

#include "GameTechRenderer.h"
#include "PhysicsSystem.h"

#include "ToonLevelManager.h"
#include <reactphysics3d/reactphysics3d.h>
#include "PaintBallClass.h"
#include "ToonEventListener.h"
#include "ToonDebugManager.h"

namespace NCL
{
	namespace CSC8503
	{
		class ToonFollowCamera;
		class ToonGame
		{
		public:
			ToonGame();
			~ToonGame();

			virtual void UpdateGame(float dt);			

		protected:
			void UpdateCamera(float dt);
			void UpdateTesting();

		protected:
			ToonFollowCamera* followCamera;
			Player*	player = nullptr;
			ToonGameObject* targetObject = nullptr;

			PaintableZone* mainZone;
			std::vector<PaintableZone*>* subZones; // TODO: This can maybe be stored better.... only doing as vector for easy delete

			PaintBallClass* sampleWeapon;
			GameTechRenderer* renderer;
			ToonGameWorld* world;
			ToonLevelManager* levelManager;
			PaintBallClass* baseWeapon;
			ToonEventListener* eventListener;
			Team* testTeam;

			bool showCursor;
			const float timeStep = 1.0f / 60.0f;
			float accumulator;

			ToonDebugManager* toonDebugManager;
			std::chrono::steady_clock::time_point lastFrameTime;
		};
	}
}