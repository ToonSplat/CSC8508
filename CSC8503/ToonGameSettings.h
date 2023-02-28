#pragma once
#include "PushdownMachine.h"
#include "PushdownState.h"
#include "GameTechRenderer.h"
#include "ToonToggleButton.h"

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

		SettingsDataStructure(Coordinates coord, std::string txt, bool hasToggle = true, ToggleButtonStates toggleState = ToggleButtonStates::ToggleOff, Coordinates toggleCoord = Coordinates())
		{
			coordinates					  = coord;
			text						  = txt;
			this->hasToggle				  = hasToggle;
			Coordinates toggleCoordinates = toggleCoord == Coordinates() ? Coordinates(Vector2(coordinates.origin.x + coordinates.size.x - 20.0f, coordinates.origin.y - 3.0f), Vector2(8.0f, coordinates.size.y)) : toggleCoord;
			toggleButton				  = new ToonToggleButton(toggleCoordinates, toggleState, true);
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
		std::vector<SettingsDataStructure> m_SettingsData		   = {
																		SettingsDataStructure(Coordinates(Vector2(5.0f, 20.0f), Vector2(80.0f, 10.0f)), "Invert Camera"),
																		SettingsDataStructure(Coordinates(Vector2(5.0f, 30.0f), Vector2(80.0f, 10.0f)), "Enable Shadow", true, ToggleButtonStates::ToggleOn),
																		SettingsDataStructure(Coordinates(Vector2(5.0f, 40.0f), Vector2(80.0f, 10.0f)), "Resize Window", false),
																		SettingsDataStructure(Coordinates(Vector2(5.0f, 50.0f), Vector2(80.0f, 10.0f)), "Back",		     false)
																	 };

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
};