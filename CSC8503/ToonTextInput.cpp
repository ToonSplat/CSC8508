#include "ToonTextInput.h"

ToonTextInput::ToonTextInput(Coordinates coordinates, Vector4 focusColour, Vector4 unfocusColour)
{
	m_Coordinates	= coordinates;
	m_IsFocused		= false;
	m_FocusColour	= focusColour;
	m_UnfocusColour = unfocusColour;
	m_InputText		= "";
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
	
}