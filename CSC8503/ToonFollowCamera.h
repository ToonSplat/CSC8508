#pragma once
#include "Camera.h"
#include "GameTechRenderer.h"
#include "ToonGameObject.h"
#include "Player.h"

namespace NCL
{
	namespace CSC8503
	{
		class ToonFollowCamera : public Camera
		{
		public:
			ToonFollowCamera(ToonGameObject* target);
			~ToonFollowCamera() {};

			virtual void UpdateCamera(float dt) override;

			float GetPitchOffset() const { return pitchOffset; }
			void SetPitchOffset(const float& newPitchOffset) { pitchOffset = newPitchOffset; }

			float GetFollowDistance() const { return requiredRayDistance; }
			void SetFollowDistance(const float& newDistance) { requiredRayDistance = newDistance; }

			Vector3 GetFollowOffset() const { return followOffset; }
			void SetFollowOffset(const Vector3& newOffset) { followOffset = newOffset; }

			Vector3 GetTargetOffset() const { return targetOffset; }
			void SetTargetOffset(const Vector3& newOffset) { targetOffset = newOffset; }

			Vector3 GetAimOffset() const { return aimOffset; }
			void SetAimOffset(const Vector3& newOffset) { aimOffset = newOffset; }

		protected:
			ToonGameObject* followTarget;
			Player* player;

			float pitchOffset;

		private:
			float requiredRayDistance, defaultRayDistance;
			Vector3 followOffset, targetOffset, aimOffset;
			Vector3 up, right;

			float h, v;
			bool hittingWall;

			Vector3 refVel;
		};
	}
}