#pragma once
#include <vector>
#include <reactphysics3d/reactphysics3d.h>
#include <unordered_set>
#include "HitSphere.h"

namespace NCL
{
	class Camera;
	namespace CSC8503
	{
		class ToonGameObject;
		class ToonEventListener;
		class PaintBallProjectile;
		class PaintableObject;
		class Team;
		typedef std::function<void(ToonGameObject*)> ToonGameObjectFunc;

		class ToonGameWorld
		{
		public:
			ToonGameWorld();
			~ToonGameWorld();

			void Clear();
			void ClearAndErase();

			void AddEventListener(ToonEventListener* eventListener) { this->eventListener = eventListener; }

			void AddGameObject(ToonGameObject* o);
			void RemoveGameObject(ToonGameObject* o, bool andDelete = false);

			void AddPaintball(PaintBallProjectile* paintball);
			void RemovePaintball(PaintBallProjectile* paintball);
			std::unordered_set<PaintBallProjectile*> GetPaintballs(void) const { return activePaintballs; }

			void AddHitSphere(HitSphere* hitSphere);
			void RemoveHitSphere(HitSphere* hitSphere);
			std::unordered_set<HitSphere*> GetHitSpheres(void) const { return activeHitSpheres; }

			void AddPaintableObject(PaintableObject* paintableObject);
			void RemovePaintableObject(PaintableObject* paintableObject);
			std::unordered_set<PaintableObject*> GetPaintableObjects(void) const { return paintableObjects; }

			void DeleteMarkedObjects();

			void GetGameObjects(void) const;

			Camera* GetMainCamera() const { return mainCamera; }
			void SetMainCamera(Camera* newCamera) { 
				delete mainCamera;
				mainCamera = newCamera; 
			}

			Camera* GetMinimapCamera() const { return minimapCamera; }
			void SetMinimapCamera(Camera* newCamera) { minimapCamera = newCamera; }

			virtual void UpdateWorld(float dt);
			void OperateOnContents(ToonGameObjectFunc f);
      
			bool ShowCursor() const { return showCursor; }

			Team* GetTeamLeastPlayers();

			reactphysics3d::PhysicsWorld& GetPhysicsWorld() const { return *physicsWorld; }
			reactphysics3d::PhysicsCommon& GetPhysicsCommon() { return physicsCommon; }

		protected:
			Camera* mainCamera;
			Camera* minimapCamera;
			reactphysics3d::PhysicsCommon physicsCommon;
			reactphysics3d::PhysicsWorld* physicsWorld;
			ToonEventListener* eventListener;

			std::vector<ToonGameObject*> gameObjects;
			std::unordered_set<PaintBallProjectile*> activePaintballs;
			std::unordered_set<HitSphere*> activeHitSpheres;
			std::unordered_set<PaintableObject*> paintableObjects;
			std::unordered_set<ToonGameObject*> objectsToDelete;

			std::set<Team*> teams;

			int		worldIDCounter;
			int		worldStateCounter;

		private:
			static ToonGameWorld* instance;
			bool showCursor;
		};
	}
}