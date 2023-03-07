#pragma once
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
		float distanceFromFocus = 30.0f;
	};
}
