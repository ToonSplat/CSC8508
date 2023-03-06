#pragma once
#include "ToonFollowCamera.h"
#include "GameTechRenderer.h"
#include "ToonGameObject.h"


namespace NCL::CSC8503 {
	class ToonMapCamera : public Camera {
	public:
		ToonMapCamera();
		~ToonMapCamera() {}

		void UpdateCamera(float dt, BaseInput* inputs) override;
	protected:
		float zoomFactor = 5.0f;
		float distanceFromFocus = 300.0f;
	};
}
