#pragma once
#include "BaseInput.h"
#include "GameTechRenderer.h"

namespace NCL
{
	namespace CSC8503
	{
		class ToonSettingsManager
		{
		public:
			static void ApplySettings() {
				std::ifstream file = std::ifstream(Assets::DATADIR + "ToonSettings.txt");
				if (!file.is_open()) {
					std::cerr << "Failed to open the file\n";
				}
				std::string line;

				vector<string> result;
				while (std::getline(file, line)) {
					size_t pos = 0;
					while (pos != string::npos) {
						size_t next_pos = line.find(':', pos);
						if (next_pos == string::npos) {
							result.push_back(line.substr(pos));
							break;
						}
						result.push_back(line.substr(pos, next_pos - pos));
						pos = next_pos + 1;
					}
				}

				if (result[0] == "InvertCamera")
					BaseInput::SetInverted(result[1] == "1");
			}
		protected:
			ToonSettingsManager() {}
			~ToonSettingsManager() {}
		};
	}
}