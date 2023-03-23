#include "ToonSettingsManager.h"
#include "AudioSystem.h"
#include "BaseInput.h"
#include "ToonGameSettings.h"
#include "GameTechRenderer.h"
#include "PlayerNPC.h"

GameTechRenderer* NCL::CSC8503::ToonSettingsManager::renderer = nullptr;

void ToonSettingsManager::ApplySettings() {
	std::ifstream file = std::ifstream(Assets::DATADIR + "ToonSettings.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open the settings file\n";
	}
	std::string line;

	vector<string> result;
	while (std::getline(file, line)) {
		size_t pos = 0;
		result.clear();
		while (pos != string::npos) {
			size_t next_pos = line.find(':', pos);
			if (next_pos == string::npos) {
				result.push_back(line.substr(pos));
				break;
			}
			result.push_back(line.substr(pos, next_pos - pos));
			pos = next_pos + 1;
		}
		if (result[0] == INVERT_CAMERA_STRING)
			BaseInput::SetInverted(result[1] == "1");
		else if (result[0] == SHADOW_STRING) {
			renderer->SetShadowSize((result[1] == "1" ? 8092 : 2048));
			renderer->GenerateShadowFBO();
		}
		else if (result[0] == CROSSHAIR_STRING) {
			renderer->m_EnableDynamicCrosshair = result[1] == "1";
		}
		else if (result[0] == VSYNC_STRING) {
			renderer->SetVerticalSync(result[1] == "0" ? VerticalSyncState::VSync_OFF : VerticalSyncState::VSync_ON);
		}
		else if (result[0] == DIFFICULTY_STRING) {
			PlayerNPC::SetAIShootSpeed(result[1] == "0" ? 0.33f : 1.0f);
		}
		else if (result[0] == WINDOW_SIZE_STRING) {
			//Window::GetWindow()->SetFullScreen(result[1] == "1"); //TODO: This messes with GetMouseLocation
		}
		else if (result[0] == VOLUME_SLIDER_STRING) {
			std::string volumeString = result[1].empty() ? "10" : result[1];
			AudioSystem::GetAudioSystem()->SetMasterVolume(stoi(volumeString) / 10.0f);
		}
		else if (result[0] == FOV_SLIDER_STRING) {
			InputManager::GetInstance().SetFOV(stoi(result[1]));
		}
	}
}