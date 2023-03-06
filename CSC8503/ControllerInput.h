#pragma once
#include "BaseInput.h"

namespace NCL {
	class ControllerInput : public BaseInput {
	public:
		ControllerInput(int index) : index(index) {}
		void UpdateState() override;
	protected:
		Vector2 GetStickMovement(float x, float y);
		int index;
	};
}