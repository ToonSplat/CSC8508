#pragma once
#include "BaseInput.h"
#include "GameTechRenderer.h"
#include "ToonGameSettings.h"

namespace NCL
{
	namespace CSC8503
	{
		class ToonSettingsManager
		{
		public:
			static void ApplySettings() {
				std::ifstream file = std::ifstream(Assets::GetDataDir() + "ToonSettings.txt");
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
				}
			}
			static void SetRenderer(GameTechRenderer* newRenderer) { renderer = newRenderer; }
		protected:
			static GameTechRenderer* renderer;
			ToonSettingsManager() {}
			~ToonSettingsManager() {}
		};
	}
}