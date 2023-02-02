#pragma once
#include "Camera.h"
#include "GameTechRenderer.h"
#include "ToonGameObject.h"
#include <reactphysics3d/reactphysics3d.h>

namespace NCL
{
	namespace CSC8503
	{
		class ToonFollowCamera : public Camera
		{
		public:
			ToonFollowCamera(ToonGameWorld& gWorld, ToonGameObject& target, reactphysics3d::PhysicsWorld& _physicsWorld);
			~ToonFollowCamera() {};

			virtual void UpdateCamera(float dt) override;

		protected:
			ToonGameWorld& gameWorld;
			ToonGameObject& followTarget;
			reactphysics3d::PhysicsWorld& physicsWorld;

			float pitchOffset;

		private:
			float requiredRayDistance, defaultRayDistance;
			float h, v;
		};
	}
}