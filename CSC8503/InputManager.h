#pragma once
#include "BaseInput.h"
#include "KeyboardInput.h"
namespace NCL {
	using namespace Maths;

	class InputManager {
	public:
		static InputManager& GetInstance() {
			static InputManager instance;
			return instance;
		}

		void Update() {
			for (auto& [id, input] : playerInputs){
				input->UpdateState();
			}
		}

		void AddInput(int player, BaseInput* inputScheme) {
			if (playerInputs.find(player) != playerInputs.end())
				delete playerInputs.find(player)->second;
			playerInputs[player] = inputScheme;
		}

		std::map<int, BaseInput*>& GetInputs() { return playerInputs; }

	private:
		InputManager() {
			// Initialize player inputs here
		}

		~InputManager() {
			for (auto& [id, input] : playerInputs) {
				delete input;
			}
		}

		std::map<int, BaseInput*> playerInputs;
	};
}