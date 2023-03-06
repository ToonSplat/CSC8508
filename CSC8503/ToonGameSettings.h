#pragma once
#include "PushdownMachine.h"
#include "PushdownState.h"
#include "GameTechRenderer.h"
#include "ToonToggleButton.h"
#include "ToonFileHandling.h"
#include "Assets.h"
#include <unordered_map>


#define INVERT_CAMERA_STRING "InvertCamera"
#define SHADOW_STRING		 "Shadow"
#define WINDOW_SIZE_STRING	 "WindowSize"

using namespace NCL;
//using namespace CSC8503;

class ToonGameSettings : public PushdownState
{
	enum SettingsScreenStates
	{
		InvertCamera,
		Shadow,
		WindowSize,
		SettingsBack
	};

	struct SettingsDataStructure
	{
		Coordinates		  coordinates;
		std::string		  text;
		bool			  hasToggle;
		ToonToggleButton* toggleButton = NULL;
		Vector2			  windowSize;

		SettingsDataStructure(Coordinates coord, Vector2 windowSize, std::string txt, bool hasToggle = true, int toggleButtonID = -1, ToggleButtonStates toggleState = ToggleButtonStates::ToggleOff, std::vector<std::string>toggleValueText = { "OFF", "ON" }, Coordinates toggleCoord = Coordinates())
		{
			coordinates					  = coord;
			text						  = txt;
			this->hasToggle				  = hasToggle;
			this->windowSize			  = windowSize;
			Coordinates toggleCoordinates = toggleCoord == Coordinates() ? Coordinates(Vector2(coordinates.origin.x + coordinates.size.x - 20.0f, coordinates.origin.y - 3.0f), Vector2(8.0f, coordinates.size.y)) : toggleCoord;
			toggleButton = new ToonToggleButton(toggleCoordinates, windowSize, toggleButtonID, toggleState, true, toggleValueText);
		}
	};

	struct ToonSettingsFileDataStructure
	{
		ToggleButtonStates invertCameraState = ToggleButtonStates::ToggleOff;
		ToggleButtonStates shadowState	     = ToggleButtonStates::ToggleOff;
		std::string windowSize				 = "";

		std::unordered_map<std::string, std::string> SeperateComponents(const std::string& dataString, char delimiter = ':')
		{
			std::unordered_map<std::string, std::string> parsedComponentsMap;
			int											 index   = 0;
			bool										 isValue = false;
			std::string currentString[2] = { "", "" };
			while (dataString[index] != '\0')
			{
				if (dataString[index] == delimiter)
				{
					isValue = !isValue;
				}
				else if (dataString[index] == '\n')
				{
					parsedComponentsMap[currentString[0]] = currentString[1];
					isValue								  = !isValue;
					currentString[0]					  = "";
					currentString[1]					  = "";
				}
				else
				{
					currentString[isValue] += dataString[index];
				}
				index++;
			}
			return parsedComponentsMap;
		}

		void ParseData(const std::string& dataString)
		{
			std::unordered_map<std::string, std::string> parsedMap = SeperateComponents(dataString);
			for (std::pair<std::string, std::string> it : parsedMap)
			{
				if		(it.first == INVERT_CAMERA_STRING) { invertCameraState = it.second == "1" ? ToggleButtonStates::ToggleOn : ToggleButtonStates::ToggleOff; }
				else if (it.first == SHADOW_STRING)		   { shadowState	   = it.second == "1" ? ToggleButtonStates::ToggleOn : ToggleButtonStates::ToggleOff; }
				else if (it.first == WINDOW_SIZE_STRING)   { windowSize		   = it.second; }
			}
		}

		std::string SerializeStructure()
		{
			std::string serializedString = INVERT_CAMERA_STRING + std::string(":") + std::string((invertCameraState == ToggleButtonStates::ToggleOff ? "0" : "1")) + std::string("\n");
			serializedString		    += SHADOW_STRING + std::string(":") + std::string((shadowState == ToggleButtonStates::ToggleOff ? "0" : "1")) + std::string("\n");
			serializedString			+= WINDOW_SIZE_STRING + std::string(":") + windowSize + std::string("\n");
			return serializedString;
		}
	};

	private:
		GameTechRenderer*				   m_Renderer;
		ToonGameWorld*					   m_World;
		Window*							   m_Window				   = NULL;
		Vector4							   m_SelectedColour		   = Debug::GREEN;
		Vector4							   m_NonSelectedColour	   = Debug::WHITE;
		int								   m_CurrentSelectedIndex  = 0;
		bool							   m_IsMousePointerVisible = false;
		Vector2							   m_MouseLastPosition	   = Vector2();
		std::vector<SettingsDataStructure> m_SettingsData;
		ToonFileHandling*				   m_SettingsFile		   = NULL;
		ToonSettingsFileDataStructure	   m_SettingsDS;
		const std::string&				   Settings_File_Name	   = NCL::Assets::DATADIR + "ToonSettings.txt";

	public:
		ToonGameSettings(GameTechRenderer* renderer, ToonGameWorld* world, Window* win);
		~ToonGameSettings();
		PushdownResult OnUpdate(float dt, PushdownState** newState) override;
		void OnAwake() override;
		void OnSleep() override;

	private:
		void DrawScreen();
		void UpdateCurrentSelectedIndex(int incrementBy);
		void HandleKeyboardAndMouseEvents();
		void UpdateMosePointerState(bool isVisible);
		void WakeMouseOnMovement();
		bool isInside(Vector2 mousePosition, Coordinates menuDataCoordinates);
		PushdownState::PushdownResult HandleNavigation(PushdownState** newState);
		void FreeAllToonToggleButtons();
		void PopulateSettingsData();
		void UpdateSettingsFile();
};