#pragma once
#include "ToonFollowCamera.h"
#include "GameTechRenderer.h"
#include "ToonGameObject.h"
#include "BaseInput.h"

namespace NCL::CSC8503 {
	class ToonMinimapCamera : public Camera {
	public:
		ToonMinimapCamera(ToonGameObject& target);
		~ToonMinimapCamera() {}

		void UpdateCamera(float dt, BaseInput* inputs) override;
	protected:
		ToonGameObject& followTarget;
		float zoomFactor = 15.0f;
		float distanceFromFocus = 30.0f;
	};
}
