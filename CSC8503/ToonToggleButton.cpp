#include "ToonToggleButton.h"

void ToonToggleButton::DrawButton()
{
	DrawOffToggle();
	DrawOnToggle();
	DrawHead();
	if (m_ShouldDrawValue) { DrawValue(); }
}


ToonToggleButton::ToonToggleButton(Coordinates coordinates, bool shouldDrawValue)
{
	m_CurrentState	  = ToggleButtonStates::ToggleOff;
	m_Coordinates	  = coordinates;
	m_HalfSize		  = m_Coordinates;
	m_HalfSize.size	  = m_Coordinates.size / 2;
	m_ShouldDrawValue = shouldDrawValue;
}

ToonToggleButton::~ToonToggleButton()
{
	
}

ToggleButtonStates ToonToggleButton::GetButtonState()
{
	return m_CurrentState;
}

void ToonToggleButton::UpdateButtonDraw()
{
	HandleEvents();
	DrawButton();
}

void ToonToggleButton::HandleEvents()
{
	if (InputManager::GetInstance().GetInputs()[1]->IsShooting()) { ToggleState(); }
}

void ToonToggleButton::DrawOffToggle()
{
	Debug::DrawFilledQuad(m_HalfSize.origin, m_HalfSize.size, m_ToggleOnColour);
}

void ToonToggleButton::DrawOnToggle()
{
	Vector2 origin = m_Coordinates.origin;
	origin.x	  += m_HalfSize.size.x;
	Debug::DrawFilledQuad(origin, m_HalfSize.size, m_ToggleOffColour);
}

void ToonToggleButton::DrawHead()
{
	Coordinates headCoordinates = m_CurrentState == ToggleButtonStates::ToggleOff ? m_HalfSize : Coordinates(Vector2(m_Coordinates.origin.x + m_HalfSize.size.x, m_Coordinates.origin.y), m_HalfSize.size);
	headCoordinates.ScaleCoordinatesBy(0.15f);
	Debug::DrawFilledQuad(headCoordinates.origin, headCoordinates.size, m_HeadColour);
	/*headCoordinates.ScaleCoordinatesBy(0.15f);
	Debug::DrawQuad(headCoordinates.origin, headCoordinates.size, Vector4(1.0f, 1.0f, 1.0f, 1.0f));*/
}

void ToonToggleButton::ToggleState()
{
	m_CurrentState = m_CurrentState == ToggleButtonStates::ToggleOff ? ToggleButtonStates::ToggleOn : ToggleButtonStates::ToggleOff;
}

void ToonToggleButton::DrawValue()
{
	Coordinates valueCoordinates = m_Coordinates;
	valueCoordinates.origin.x	+= valueCoordinates.size.x + 2.0f;
	valueCoordinates.origin.y	+= (valueCoordinates.size.y / 2.0f) - 1.0f;
	std::string toggleValue		 = m_CurrentState == ToggleButtonStates::ToggleOff ? "OFF" : "ON";
	Debug::Print(toggleValue, valueCoordinates.origin, Debug::WHITE);
}