#include "ToonTextInput.h"

ToonTextInput::ToonTextInput(Coordinates coordinates, Vector2 windowSize, ToonVirtualKeyboard::KeyboardInputType inputType, Vector4 focusColour, Vector4 unfocusColour) : m_Coordinates(coordinates), m_WindowSize(windowSize), m_InputType(inputType), m_IsFocused(true), m_FocusColour(focusColour), m_UnfocusColour(unfocusColour), m_InputText("")
{
	Coordinates keyboardCoordinates  = m_Coordinates;
	keyboardCoordinates.origin.y	+= keyboardCoordinates.size.y;
	m_VirtualKeyboard				 = new ToonVirtualKeyboard(keyboardCoordinates, m_WindowSize, ToonVirtualKeyboard::KeyboardInputType::Alphabetic);
}

ToonTextInput::~ToonTextInput()
{
	
}

void ToonTextInput::UpdatePosition(Coordinates newCoordinates)
{
	m_Coordinates = newCoordinates;
}

void ToonTextInput::Update()
{
	Debug::DrawQuad(m_Coordinates.origin, m_Coordinates.size, Debug::BLUE);
	//if (m_IsFocused) { DrawVirtualKeyboard(); }
	if (m_IsFocused) { m_VirtualKeyboard->UpdateAndHandleInputEvents(); }
}

void ToonTextInput::DrawVirtualKeyboard()
{
	// Test
	float startX	 = m_Coordinates.origin.x;
	float startY	 = m_Coordinates.origin.y;
	float buttonSize = 5.0f;
	for (int i = 'A'; i <= 'Z'; i++)
	{
		if (startX + buttonSize > m_Coordinates.origin.x + m_Coordinates.size.x)
		{
			startX  = m_Coordinates.origin.x;
			startY += buttonSize;
		}
		std::string keyString	  = "";
		keyString				 += i;
		Coordinates keyCoordinate = Coordinates(Vector2(startX, startY), Vector2(buttonSize, buttonSize));
		DrawKeyboardKey(keyCoordinate, keyString, Debug::GREEN);
		startX += buttonSize;
	}
}

void ToonTextInput::DrawKeyboardKey(Coordinates coordinate, std::string key, Vector4 keyColour)
{
	Debug::DrawQuad(coordinate.origin, coordinate.size, m_UnfocusColour);
	const float	   offset			 = 1.0f;
	const Vector2& characterPosition = Vector2(coordinate.origin.x + (coordinate.size.x / 2) - offset, coordinate.origin.y + (coordinate.size.y / 2) + offset);
	Debug::Print(key, characterPosition, keyColour);
}