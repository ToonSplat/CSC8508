#pragma once
#include "ToonFollowCamera.h"
#include "GameTechRenderer.h"
#include "ToonGameObject.h"


namespace NCL::CSC8503 {
	class ToonMinimapCamera : public Camera {
	public:
		ToonMinimapCamera(ToonGameObject& target);
		~ToonMinimapCamera() {}

		void UpdateCamera(float dt) override;
	protected:
		ToonGameObject& followTarget;
		float zoomFactor = 10.0f;
		float distanceFromFocus = 50.0f;
	};
}
