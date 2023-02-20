#include "ToonConfirmationScreen.h"

ToonConfirmationScreen::ToonConfirmationScreen(Coordinates coordinates,
											   Vector2 windowSize,
											   std::string text,
											   Vector4 textColour,
											   std::string okButtonText,
											   std::string noButtonText) : m_Coordinates(coordinates),
																		   m_WindowSize(windowSize),
																		   m_Text(text),
																		   m_TextColour(textColour),
																		   m_OkButton(okButtonText,		EMPTY_COORDINATES, ConfirmationButtonsType::Ok),
																		   m_CancelButton(okButtonText, EMPTY_COORDINATES, ConfirmationButtonsType::Cancel),
																		   m_CurrentSelectedButton(ConfirmationButtonsType::None)
{
	UpdateButtonsCoordinates();
}

void ToonConfirmationScreen::Update()
{
}

void ToonConfirmationScreen::UpdateButtonsCoordinates()
{
	m_OkButton.buttonCoordinates	 = Coordinates(Vector2(m_Coordinates.origin.x, m_Coordinates.origin.y + 20.0f), Vector2(CONFIRMATION_BUTTON_WIDTH, CONFIRMATION_BUTTON_HEIGHT));
	m_CancelButton.buttonCoordinates = Coordinates(Vector2(m_Coordinates.origin.x + m_Coordinates.size.x - CONFIRMATION_BUTTON_WIDTH, m_Coordinates.origin.y + 20.0f), Vector2(CONFIRMATION_BUTTON_WIDTH, CONFIRMATION_BUTTON_HEIGHT));
}

void ToonConfirmationScreen::DrawScreen()
{
	Debug::Print(m_Text, m_Coordinates.origin, m_TextColour);
	DrawSingleButton(ConfirmationButtonsType::Ok);
	DrawSingleButton(ConfirmationButtonsType::Cancel);
}

void ToonConfirmationScreen::DrawSingleButton(ConfirmationButtonsType buttonType)
{
	Button button = buttonType == ConfirmationButtonsType::Ok ? m_OkButton : m_CancelButton;
	float offset  = 1.0f;
	if (m_IsMousePointerVisible)
	{
		Vector2 mousePosition = Window::GetMouse()->GetWindowPosition();
		float	y			  = ((mousePosition.y / m_WindowSize.y) * 100);
		float	x			  = ((mousePosition.x / m_WindowSize.x) * 100);
		bool	isHovered	  = button.buttonCoordinates.origin.x <= x && button.buttonCoordinates.origin.x + button.buttonCoordinates.size.x >= x && button.buttonCoordinates.origin.y <= y && button.buttonCoordinates.origin.y + button.buttonCoordinates.size.y >= y;
		Debug::DrawQuad(button.buttonCoordinates.origin, button.buttonCoordinates.size, isHovered ? m_SelectedTextColour : m_TextColour);
		Debug::Print(button.buttonText, button.buttonCoordinates.origin + Vector2(offset, offset), isHovered ? m_SelectedTextColour : m_TextColour);
	}
}