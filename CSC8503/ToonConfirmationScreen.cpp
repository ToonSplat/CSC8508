#include "ToonConfirmationScreen.h"

ToonConfirmationScreen::ToonConfirmationScreen() : m_Coordinates(Vector2(0, 0), Vector2(0, 0)), m_OkButton("", EMPTY_COORDINATES, ConfirmationButtonsType::Ok), m_CancelButton("", EMPTY_COORDINATES, ConfirmationButtonsType::Cancel) {}

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
																		   m_CancelButton(noButtonText, EMPTY_COORDINATES, ConfirmationButtonsType::Cancel),
																		   m_CurrentSelectedButton(ConfirmationButtonsType::None)
{
	UpdateButtonsCoordinates();
}

//void ToonConfirmationScreen::Update()
//{
//	m_CurrentSelectedButton = ConfirmationButtonsType::None;
//	DrawScreen();
//	if (Window::GetMouse()->ButtonPressed(MouseButtons::LEFT))
//	{
//		switch (m_CurrentSelectedButton)
//		{
//			case Ok:
//				delegate->DidSelectOkButton();
//				break;
//			case Cancel:
//				delegate->DidSelectCancelButton();
//				break;
//			case None:
//				break;
//		}
//	}
//}

PushdownState::PushdownResult ToonConfirmationScreen::OnUpdate(float dt, PushdownState** newState)
{
	m_CurrentSelectedButton = ConfirmationButtonsType::None;
	DrawScreen();
	if (Window::GetMouse()->ButtonPressed(MouseButtons::LEFT))
	{
		switch (m_CurrentSelectedButton)
		{
		case Ok:
			return delegate->DidSelectOkButton();
			break;
		case Cancel:
			return delegate->DidSelectCancelButton();
			break;
		case None:
			break;
		}
	}
	return PushdownState::PushdownResult::NoChange;
}

void ToonConfirmationScreen::OnSleep()
{
}

void ToonConfirmationScreen::OnAwake()
{
}

void ToonConfirmationScreen::UpdateButtonsCoordinates()
{
	m_OkButton.buttonCoordinates	 = Coordinates(Vector2(m_Coordinates.origin.x, m_Coordinates.origin.y + 20.0f), Vector2(CONFIRMATION_BUTTON_WIDTH, CONFIRMATION_BUTTON_HEIGHT));
	m_CancelButton.buttonCoordinates = Coordinates(Vector2(m_Coordinates.origin.x + m_Coordinates.size.x - CONFIRMATION_BUTTON_WIDTH, m_Coordinates.origin.y + 20.0f), Vector2(CONFIRMATION_BUTTON_WIDTH, CONFIRMATION_BUTTON_HEIGHT));
}

void ToonConfirmationScreen::DrawScreen()
{
	Debug::Print(m_Text, Vector2(m_Coordinates.origin.x + (m_Coordinates.size.x / 2.0f) - m_Text.length(), m_Coordinates.origin.y), m_TextColour);
	DrawSingleButton(ConfirmationButtonsType::Cancel);
	DrawSingleButton(ConfirmationButtonsType::Ok);
}

void ToonConfirmationScreen::DrawSingleButton(ConfirmationButtonsType buttonType)
{
	Button button = buttonType == ConfirmationButtonsType::Ok ? m_OkButton : m_CancelButton;
	float xOffset = (CONFIRMATION_BUTTON_WIDTH / 2.0f) - button.buttonText.length();
	float yOffset = (CONFIRMATION_BUTTON_HEIGHT / 2.0f) + 1.0f;
	if (m_IsMousePointerVisible)
	{
		Vector2 mousePosition = Window::GetMouse()->GetWindowPosition();
		float	y			  = ((mousePosition.y / m_WindowSize.y) * 100);
		float	x			  = ((mousePosition.x / m_WindowSize.x) * 100);
		bool	isHovered	  = button.buttonCoordinates.origin.x <= x && button.buttonCoordinates.origin.x + button.buttonCoordinates.size.x >= x && button.buttonCoordinates.origin.y <= y && button.buttonCoordinates.origin.y + button.buttonCoordinates.size.y >= y;
		Debug::DrawQuad(button.buttonCoordinates.origin, button.buttonCoordinates.size, isHovered ? m_SelectedTextColour : m_TextColour);
		Debug::Print(button.buttonText, button.buttonCoordinates.origin + Vector2(xOffset, yOffset), isHovered ? m_SelectedTextColour : m_TextColour);
		if (isHovered) { m_CurrentSelectedButton = button.identifier; }
	}
}