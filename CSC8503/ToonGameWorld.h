#pragma once
#include <vector>

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

			virtual void UpdateWorld(float dt);
			void OperateOnContents(ToonGameObjectFunc f);

		protected:
			Camera* mainCamera;

			std::vector<ToonGameObject*> gameObjects;

			int		worldIDCounter;
			int		worldStateCounter;

		private:
			static ToonGameWorld* instance;
		};
	}
}