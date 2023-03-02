#pragma once
#include "BaseInput.h"

namespace NCL {
	class XboxControllerInput : public BaseInput {
	public:
		XboxControllerInput(int index) : index(index) {}
		void UpdateState() override;
	protected:
		Vector2 GetStickMovement(float x, float y);
		int index;
	};
}