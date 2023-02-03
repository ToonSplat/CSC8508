#pragma once
#include <vector>
#include <reactphysics3d/reactphysics3d.h>

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

			static ToonGameWorld* Get() { return instance; }

			Camera* GetMainCamera() const { return mainCamera; }
			void SetMainCamera(Camera* newCamera) { mainCamera = newCamera; }

			Camera* GetMinimapCamera() const { return minimapCamera; }
			void SetMinimapCamera(Camera* newCamera) { minimapCamera = newCamera; }

			virtual void UpdateWorld(float dt);
			void OperateOnContents(ToonGameObjectFunc f);

			reactphysics3d::PhysicsWorld& GetPhysicsWorld() const { return *physicsWorld; }
			reactphysics3d::PhysicsCommon& GetPhysicsCommon() { return physicsCommon; }

		protected:
			Camera* mainCamera;
			Camera* minimapCamera;
			reactphysics3d::PhysicsCommon physicsCommon;
			reactphysics3d::PhysicsWorld* physicsWorld;

			std::vector<ToonGameObject*> gameObjects;

			int		worldIDCounter;
			int		worldStateCounter;

		private:
			static ToonGameWorld* instance;
		};
	}
}