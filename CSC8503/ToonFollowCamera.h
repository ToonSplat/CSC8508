#pragma once
#include "Camera.h"
#include "GameTechRenderer.h"
#include "ToonGameObject.h"

namespace NCL
{
	namespace CSC8503
	{
		class ToonFollowCamera : public Camera
		{
		public:
			ToonFollowCamera(ToonGameObject& target);
			~ToonFollowCamera() {};

			virtual void UpdateCamera(float dt) override;

			float GetFollowDistance() const { return requiredRayDistance; }
			void SetFollowDistance(const float& newDistance) { requiredRayDistance = newDistance; }

			Vector3 GetFollowOffset() const { return followOffset; }
			void SetFollowOffset(const Vector3& newOffset) { followOffset = newOffset; }

			Vector3 GetTargetOffset() const { return targetOffset; }
			void SetTargetOffset(const Vector3& newOffset) { targetOffset = newOffset; }

		protected:
			ToonGameObject& followTarget;

			float pitchOffset;

		private:
			float requiredRayDistance, defaultRayDistance;
			Vector3 followOffset, targetOffset;
			Vector3 up;
			float h, v;
			float radianX, radianY;
		};
	}
}