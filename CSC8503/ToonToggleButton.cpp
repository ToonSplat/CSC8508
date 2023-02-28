#include "ToonToggleButton.h"

void ToonToggleButton::DrawButton()
{
	DrawOffToggle();
	DrawOnToggle();
	m_ToggleButtonHead->UpdateHead();
	if (m_ShouldDrawValue) { DrawValue(); }
}


ToonToggleButton::ToonToggleButton(Coordinates coordinates, ToggleButtonStates currentState, bool shouldDrawValue)
{
	m_CurrentState	   = ToggleButtonStates::ToggleOff;
	m_Coordinates	   = coordinates;
	m_HalfSize		   = m_Coordinates;
	m_HalfSize.size	   = m_Coordinates.size / 2;
	m_ShouldDrawValue  = shouldDrawValue;
	m_CurrentState	   = currentState;
	m_ToggleButtonHead = new ToonToggleButtonHead(m_HalfSize, m_CurrentState);
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
	if (m_IsActive) { HandleEvents(); }
	DrawButton();
}

void ToonToggleButton::HandleEvents()
{
	if (InputManager::GetInstance().GetInputs()[1]->IsShooting() || InputManager::GetInstance().GetInputs()[1]->IsSelecting())
	{
		ToggleCurrentState();
		m_ToggleButtonHead->TriggerAnimation();
	}
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

void ToonToggleButton::DrawValue()
{
	Coordinates valueCoordinates = m_Coordinates;
	valueCoordinates.origin.x	+= valueCoordinates.size.x + 2.0f;
	valueCoordinates.origin.y	+= (valueCoordinates.size.y / 2.0f) - 1.0f;
	std::string toggleValue		 = m_CurrentState == ToggleButtonStates::ToggleOff ? "OFF" : "ON";
	Debug::Print(toggleValue, valueCoordinates.origin, Debug::WHITE);
}

void ToonToggleButton::ToggleCurrentState()
{
	m_CurrentState = m_CurrentState == ToggleButtonStates::ToggleOff ? ToggleButtonStates::ToggleOn : ToggleButtonStates::ToggleOff;
}