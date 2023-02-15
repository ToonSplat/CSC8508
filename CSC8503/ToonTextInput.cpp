#include "ToonTextInput.h"

ToonTextInput::ToonTextInput(Coordinates coordinates, ToonVirtualKeyboard::KeyboardInputType inputType, Vector4 focusColour, Vector4 unfocusColour) : m_Coordinates(coordinates), m_InputType(inputType), m_IsFocused(true), m_FocusColour(focusColour), m_UnfocusColour(unfocusColour), m_InputText("") {}

ToonTextInput::~ToonTextInput()
{
	
}

void ToonTextInput::UpdatePosition(Coordinates newCoordinates)
{
	m_Coordinates = newCoordinates;
}

void ToonTextInput::Update()
{
	if (m_IsFocused) { DrawVirtualKeyboard(); }
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