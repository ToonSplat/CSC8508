#pragma once
#include "Camera.h"

namespace NCL::CSC8503 {
	class ToonObserverCamera : public Camera {
	public:
		ToonObserverCamera(void);
		~ToonObserverCamera() {}

		void UpdateCamera(float dt, BaseInput* inputs) override;
	};
}