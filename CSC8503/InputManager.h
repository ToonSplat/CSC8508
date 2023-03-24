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

		float GetFOV() const { return fov; }
		void SetFOV(float fov) { this->fov = fov; }

		std::map<int, BaseInput*>& GetInputs() { return playerInputs; }

	private:
		InputManager() {
			fov = 60.0f;
			// Initialize player inputs here
		}

		~InputManager() {
			for (auto& [id, input] : playerInputs) {
				delete input;
			}
		}

		std::map<int, BaseInput*> playerInputs;
		float fov;
	};
}