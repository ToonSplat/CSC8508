#pragma once
#include "BaseInput.h"

namespace NCL {
	class Keyboard;
	class Mouse;
	class KeyboardInput : public BaseInput {
	public:
		KeyboardInput(const Keyboard* keyboard, const Mouse* mouse){
			this->keyboard = const_cast<Keyboard*>(keyboard);
			this->mouse = const_cast<Mouse*>(mouse);
		}
		void UpdateState() override;
	protected:
		Keyboard* keyboard;
		Mouse* mouse;
	};
}