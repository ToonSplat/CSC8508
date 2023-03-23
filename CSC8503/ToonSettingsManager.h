#pragma once

namespace NCL
{
	namespace CSC8503
	{
		class GameTechRenderer;
		class ToonSettingsManager
		{
		public:
			static void ApplySettings();
			static void SetRenderer(GameTechRenderer* newRenderer) { renderer = newRenderer; }
		protected:
			static GameTechRenderer* renderer;
			ToonSettingsManager() {}
			~ToonSettingsManager() {}
		};
	}
}