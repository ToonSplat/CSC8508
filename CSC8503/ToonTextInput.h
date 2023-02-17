#pragma once

//#include "Window.h"
//#include "ToonGame.h"
//#include "ToonGameWorld.h"
#include "ToonVirtualKeyboard.h"
#include "PushdownMachine.h"
#include "PushdownState.h"

using namespace NCL;
using namespace CSC8503;


class ToonTextInput : public PushdownState
{
	private:
		Coordinates							   m_Coordinates;
		bool								   m_IsFocused;
		Vector4								   m_FocusColour;
		Vector4								   m_UnfocusColour;
		std::string							   m_InputText;
		ToonVirtualKeyboard::KeyboardInputType m_InputType;
		Vector2								   m_WindowSize;
		ToonVirtualKeyboard*				   m_VirtualKeyboard;
		GameTechRenderer*					   m_Renderer;
		std::function<void(std::string)>	   m_doneButtonClosure;

	public:
		ToonTextInput(Coordinates coordinates, GameTechRenderer* renderer, Vector2 windowSize, std::function<void(std::string)> doneButtonClosure, ToonVirtualKeyboard::KeyboardInputType inputType = ToonVirtualKeyboard::KeyboardInputType::IPAddress, Vector4 focusColour = Debug::BLUE, Vector4 unfocusColour = Debug::WHITE);
		~ToonTextInput();
		void UpdatePosition(Coordinates newCoordinates);
		void DrawUserInputText();
		std::string GetUserInputText();
		PushdownResult OnUpdate(float dt, PushdownState** newState) override;
		void OnAwake() override;
		void OnSleep() override;
};