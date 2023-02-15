#include "ToonTextInput.h"

ToonTextInput::ToonTextInput(Coordinates coordinates, Vector2 windowSize, ToonVirtualKeyboard::KeyboardInputType inputType, Vector4 focusColour, Vector4 unfocusColour) : m_Coordinates(coordinates), m_WindowSize(windowSize), m_InputType(inputType), m_IsFocused(true), m_FocusColour(focusColour), m_UnfocusColour(unfocusColour), m_InputText("")
{
	Coordinates keyboardCoordinates  = m_Coordinates;
	keyboardCoordinates.origin.y	+= keyboardCoordinates.size.y;
	m_VirtualKeyboard				 = new ToonVirtualKeyboard(keyboardCoordinates, m_WindowSize, ToonVirtualKeyboard::KeyboardInputType::Alphabetic);
}

ToonTextInput::~ToonTextInput()
{
	delete m_VirtualKeyboard;
}

void ToonTextInput::UpdatePosition(Coordinates newCoordinates)
{
	m_Coordinates = newCoordinates;
}

void ToonTextInput::Update()
{
	Debug::DrawQuad(m_Coordinates.origin, m_Coordinates.size, Debug::BLUE);
	if (m_IsFocused) { m_VirtualKeyboard->UpdateAndHandleInputEvents(); }
}