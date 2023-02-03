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

			void SetFollowDistance(const float& newDistance) { requiredRayDistance = newDistance; }
			float GetFollowDistance() const { return requiredRayDistance; }

		protected:
			ToonGameObject& followTarget;

			float pitchOffset;

		private:
			float requiredRayDistance, defaultRayDistance;
			Vector3 followOffset;
			float h, v;
		};
	}
}