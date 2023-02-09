#pragma once
#include <vector>
#include <reactphysics3d/reactphysics3d.h>
#include "ToonEventListener.h"
#include <unordered_set>
#include "PaintBallProjectile.h"

namespace NCL
{
	class Camera;
	namespace CSC8503
	{
		class ToonGameObject;
		typedef std::function<void(ToonGameObject*)> ToonGameObjectFunc;

		class ToonGameWorld
		{
		public:
			ToonGameWorld();
			~ToonGameWorld();

			void Clear();
			void ClearAndErase();

			void AddGameObject(ToonGameObject* o);
			void RemoveGameObject(ToonGameObject* o, bool andDelete = false);

			void AddPaintball(PaintBallProjectile* paintball) {
				activePaintballs.emplace(paintball);
			}
			void RemovePaintball(PaintBallProjectile* paintball) {
				activePaintballs.erase(paintball);
				objectsToDelete.insert(paintball);
			}
			std::unordered_set<PaintBallProjectile*> GetPaintballs(void) const { return activePaintballs; }

			void AddHitSphere(HitSphere* hitSphere) {
				activeHitSpheres.emplace(hitSphere);
			}
			void RemoveHitSphere(HitSphere* hitSphere) {
				activeHitSpheres.erase(hitSphere);
				objectsToDelete.insert(hitSphere);
			}
			std::unordered_set<HitSphere*> GetHitSpheres(void) const { return activeHitSpheres; }

			void AddPaintableObject(PaintableObject* hitSphere) {
				paintableObjects.emplace(hitSphere);
			}
			void RemovePaintableObject(PaintableObject* hitSphere) {
				paintableObjects.erase(hitSphere);
				objectsToDelete.insert(hitSphere);
			}
			std::unordered_set<PaintableObject*> GetPaintableObjects(void) const { 
				return paintableObjects;
			}

			void GetGameObjects(void) const {
				for (auto& object : gameObjects)
					std::cout << object->GetRigidbody()->getUserData() << std::endl;
			}

			static ToonGameWorld* Get() { return instance; }

			Camera* GetMainCamera() const { return mainCamera; }
			void SetMainCamera(Camera* newCamera) { mainCamera = newCamera; }

			Camera* GetMinimapCamera() const { return minimapCamera; }
			void SetMinimapCamera(Camera* newCamera) { minimapCamera = newCamera; }

			virtual void UpdateWorld(float dt);
			void OperateOnContents(ToonGameObjectFunc f);

			void DeleteObjects() {
				for (auto& object : objectsToDelete)
					delete object;
				objectsToDelete.clear();
			}

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

			int		worldIDCounter;
			int		worldStateCounter;

		private:
			static ToonGameWorld* instance;
		};
	}
}