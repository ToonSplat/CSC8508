#pragma once

#include "Window.h"
//#include "PushdownMachine.h"
#include "PushdownState.h"
#include "ToonGame.h"
#include "ToonGameWorld.h"

using namespace NCL;
using namespace CSC8503;

//TODO: Use this in Main Menu screen
struct Coordinates
{
	Vector2 origin;
	Vector2 size;

	Coordinates(Vector2 org, Vector2 siz) : origin(org), size(siz) {}
};

class ToonTextInput
{
	private:
		Coordinates m_Coordinates;
		bool		m_IsFocused;
		Vector4		m_FocusColour;
		Vector4		m_UnfocusColour;
		std::string m_InputText;

	public:
		ToonTextInput(Coordinates coordinates, Vector4 focusColour = Debug::BLUE, Vector4 unfocusColour = Debug::WHITE);
		~ToonTextInput();
		void Update();
		void UpdatePosition(Coordinates newCoordinates);

	private:
		void DrawVirtualKeyboard();	//TODO: - To be drawn in a separate class.
		void DrawKeyboardKey(Coordinates coordinate, std::string key, Vector4 keyColour);
};