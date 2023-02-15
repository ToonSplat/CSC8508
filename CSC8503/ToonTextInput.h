#pragma once

//#include "Window.h"
//#include "ToonGame.h"
//#include "ToonGameWorld.h"
#include "ToonVirtualKeyboard.h"

using namespace NCL;
using namespace CSC8503;


class ToonTextInput
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

	public:
		ToonTextInput(Coordinates coordinates, Vector2 windowSize, ToonVirtualKeyboard::KeyboardInputType inputType = ToonVirtualKeyboard::KeyboardInputType::IPAddress, Vector4 focusColour = Debug::BLUE, Vector4 unfocusColour = Debug::WHITE);
		~ToonTextInput();
		void Update();
		void UpdatePosition(Coordinates newCoordinates);

	private:
		void DrawVirtualKeyboard();	//TODO: - To be drawn in a separate class.
		void DrawKeyboardKey(Coordinates coordinate, std::string key, Vector4 keyColour);
};