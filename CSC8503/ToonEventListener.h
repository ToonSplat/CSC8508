#pragma once

#include <reactphysics3d/reactphysics3d.h>

namespace NCL::CSC8503 {

	class ToonLevelManager;
	class ToonGameWorld;
	class ToonNetworkedGame;
	class ToonEventListener : public reactphysics3d::EventListener {
	public:
		ToonEventListener(reactphysics3d::PhysicsWorld* physicsWorld, ToonGameWorld* gameWorld, ToonLevelManager* levelManager);
		~ToonEventListener() {};

		void AddServer(ToonNetworkedGame* server) { this->server = server; }

        virtual void onContact(const CollisionCallback::CallbackData& callbackData) override;
        virtual void onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData) override;
	protected:
		reactphysics3d::PhysicsWorld* physicsWorld;
		ToonNetworkedGame* server;
		ToonLevelManager* levelManager;
		ToonGameWorld* gameWorld;

		int teamColourPicker = 0;
	};
}