#pragma once
#include <vector>
#include <reactphysics3d/reactphysics3d.h>
#include <unordered_set>
#include "HitSphere.h"
#include "Camera.h"
#include "Debug.h"

namespace NCL
{
	namespace CSC8503
	{
		class ToonGameObject;
		class ToonEventListener;
		class PaintBallProjectile;
		class PaintableObject;
		class Team;

		enum NetworkingStatus
		{
			Offline = 0,
			Server = 1,
			Client = 2
		};

		typedef std::function<void(ToonGameObject*)> ToonGameObjectFunc;

		class ToonGameWorld
		{
		public:
			ToonGameWorld();
			~ToonGameWorld();

			void Clear();
			void ClearAndErase();

			void AddEventListener(ToonEventListener* eventListener) { this->eventListener = eventListener; }
			ToonEventListener* GetEventListener() const { return eventListener; }

			void AddGameObject(ToonGameObject* o);
			void RemoveGameObject(ToonGameObject* o, bool andDelete = false);

			void AddPaintball(PaintBallProjectile* paintball);
			void RemovePaintball(PaintBallProjectile* paintball);
			std::unordered_set<PaintBallProjectile*> GetPaintballs(void) const { return activePaintballs; }

			void AddHitSphere(HitSphere* hitSphere);
			void RemoveHitSphere(HitSphere* hitSphere);
			std::unordered_set<HitSphere*> GetHitSpheres(void) const { return activeHitSpheres; }

			void AddPaintableObject(ToonGameObject* paintableObject);
			void RemovePaintableObject(ToonGameObject* paintableObject);
			std::unordered_set<ToonGameObject*> GetPaintableObjects(void) const { return paintableObjects; }

			void DeleteMarkedObjects();

			void GetGameObjects(void) const;

			Camera* GetMainCamera() const { return mainCamera; }
			void SetMainCamera(Camera* newCamera) { 
				delete mainCamera;
				mainCamera = newCamera; 
			}

			Camera* GetMinimapCamera() const { return minimapCamera; }
			void SetMinimapCamera(Camera* newCamera) { minimapCamera = newCamera; }
			
			Camera* GetMapCamera() const { return mapCamera; }
			void SetMapCamera(Camera* newCamera) { mapCamera = newCamera; }



			virtual void UpdateWorld(float dt);
			void OperateOnContents(ToonGameObjectFunc f);
      
			bool ShowCursor() const { return showCursor; }

			Team* GetTeamLeastPlayers();

			std::map<int, Team*>& GetTeams() { return teams; }

			NetworkingStatus GetNetworkStatus() const { return networkStatus; }
			void SetNetworkStatus(NetworkingStatus status) { networkStatus = status;; }

			reactphysics3d::PhysicsWorld& GetPhysicsWorld() const { return *physicsWorld; }
			reactphysics3d::PhysicsCommon& GetPhysicsCommon() { return physicsCommon; }

			float interpolationFactor;

		protected:
			Camera* mainCamera;
			Camera* minimapCamera;
			Camera* mapCamera;
			reactphysics3d::PhysicsCommon physicsCommon;
			reactphysics3d::PhysicsWorld* physicsWorld;
			ToonEventListener* eventListener;

			std::vector<ToonGameObject*> gameObjects;
			std::unordered_set<PaintBallProjectile*> activePaintballs;
			std::unordered_set<HitSphere*> activeHitSpheres;
			std::unordered_set<ToonGameObject*> paintableObjects;
			std::unordered_set<ToonGameObject*> objectsToDelete;

			std::map<int, Team*> teams;

			NetworkingStatus networkStatus;

			int		worldIDCounter;
			int		worldStateCounter;

		private:
			bool showCursor;
		};
	}
}