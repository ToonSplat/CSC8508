#pragma once
#include "Camera.h"
#include "GameTechRenderer.h"
#include "ToonGameObject.h"
#include "Player.h"
#include "BaseInput.h"

namespace NCL
{
	namespace CSC8503
	{
		class ToonGameWorld;
		class ToonFollowCamera : public Camera
		{
		public:
			ToonFollowCamera(ToonGameWorld* gameWorld, ToonGameObject* target, float fov = 45.0f);
			~ToonFollowCamera() {};

			virtual void UpdateCamera(float dt, BaseInput* inputs) override;

			Vector3 GetTargetOffset() const { return targetOffset; }
			void SetTargetOffset(const Vector3& newOffset) { targetOffset = newOffset; }

		protected:
			ToonGameWorld* gameWorld;
			ToonGameObject* followTarget;
			Player* player;

		private:
			Vector3 targetOffset;
			Vector3 up, right, forward;			
			
			float startFOV, aimFOV, vFov;
			float zoomSmoothess;
		};
	}
}