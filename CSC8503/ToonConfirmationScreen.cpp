#include "ToonConfirmationScreen.h"

ToonConfirmationScreen::ToonConfirmationScreen() : m_Coordinates(Coordinates()), m_OkButton("", Coordinates(), ConfirmationButtonsType::Ok), m_CancelButton("", Coordinates(), ConfirmationButtonsType::Cancel) {}

ToonConfirmationScreen::ToonConfirmationScreen(Coordinates coordinates,
											   Vector2 windowSize,
											   GameTechRenderer* renderer,
											   std::string text,
											   Vector4 textColour,
											   std::string okButtonText,
											   std::string noButtonText) : m_Coordinates(coordinates),
																		   m_WindowSize(windowSize),
																		   m_Renderer(renderer),
																		   m_Text(text),
																		   m_TextColour(textColour),
																		   m_OkButton(okButtonText, Coordinates(), ConfirmationButtonsType::Ok),
																		   m_CancelButton(noButtonText, Coordinates(), ConfirmationButtonsType::Cancel),
																		   m_CurrentSelectedButton(ConfirmationButtonsType::NoneButton)
{
	UpdateButtonsCoordinates();
}

PushdownState::PushdownResult ToonConfirmationScreen::OnUpdate(float dt, PushdownState** newState)
{
	m_Renderer->Update(dt);
	m_Renderer->Render();
	Debug::UpdateRenderables(dt);
	if (!m_IsMousePointerVisible) { WakeMouseOnMovement(); }
	if (m_IsMousePointerVisible)  { HandleMouse(); }
	HandleKeyboard();
	DrawScreen();
	if ((m_IsMousePointerVisible && Window::GetMouse()->ButtonPressed(MouseButtons::LEFT)) || (!m_IsMousePointerVisible && Window::GetKeyboard()->KeyPressed(KeyboardKeys::RETURN)))
	{
		switch (m_CurrentSelectedButton)
		{
			case Ok:
				return delegate->DidSelectOkButton();
				break;
			case Cancel:
				return delegate->DidSelectCancelButton();
				break;
			case NoneButton:
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
	UpdateMousePointerState(true);
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
	Debug::DrawQuad(button.buttonCoordinates.origin, button.buttonCoordinates.size, buttonType == m_CurrentSelectedButton ? m_SelectedTextColour : m_TextColour);
	Debug::Print(button.buttonText, button.buttonCoordinates.origin + Vector2(xOffset, yOffset), buttonType == m_CurrentSelectedButton ? m_SelectedTextColour : m_TextColour);
}

void ToonConfirmationScreen::HandleMouse()
{
	Vector2 mousePosition = Window::GetMouse()->GetWindowPosition();
	float	y			  = ((mousePosition.y / m_WindowSize.y) * 100);
	float	x			  = ((mousePosition.x / m_WindowSize.x) * 100);
	m_MouseLastPosition	  = mousePosition;
	if (m_OkButton.buttonCoordinates.origin.x <= x && m_OkButton.buttonCoordinates.origin.x + m_OkButton.buttonCoordinates.size.x >= x && m_OkButton.buttonCoordinates.origin.y <= y && m_OkButton.buttonCoordinates.origin.y + m_OkButton.buttonCoordinates.size.y >= y)
	{
		m_CurrentSelectedButton = ConfirmationButtonsType::Ok;
	}
	else if (m_CancelButton.buttonCoordinates.origin.x <= x && m_CancelButton.buttonCoordinates.origin.x + m_CancelButton.buttonCoordinates.size.x >= x && m_CancelButton.buttonCoordinates.origin.y <= y && m_CancelButton.buttonCoordinates.origin.y + m_CancelButton.buttonCoordinates.size.y >= y)
	{
		m_CurrentSelectedButton = ConfirmationButtonsType::Cancel;
	}
	else
	{
		m_CurrentSelectedButton = ConfirmationButtonsType::NoneButton;
	}
}

void ToonConfirmationScreen::HandleKeyboard()
{
	int add = 0;
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::LEFT))
	{
		add -= 1;
		UpdateMousePointerState(false);
	}
	else if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::RIGHT))
	{
		add += 1;
		UpdateMousePointerState(false);
	}
	if (!m_IsMousePointerVisible)
	{
		int currentSelectedButton = Clamp(m_CurrentSelectedButton + add, ConfirmationButtonsType::Ok, ConfirmationButtonsType::Cancel);
		m_CurrentSelectedButton   = GetConfirmationButtonTypeFromIntegerValue(currentSelectedButton);
	}
}

int ToonConfirmationScreen::Clamp(int value, int lowerBound, int upperBound)
{
	if		(value > upperBound) { return upperBound; }
	else if (value < lowerBound) { return lowerBound; }
	return value;
}

ToonConfirmationScreen::ConfirmationButtonsType ToonConfirmationScreen::GetConfirmationButtonTypeFromIntegerValue(int value)
{
	switch (value)
	{
		case 0:
			return ConfirmationButtonsType::NoneButton;
		case 1:
			return ConfirmationButtonsType::Ok;
		case 2:
			return ConfirmationButtonsType::Cancel;
	}
	return ConfirmationButtonsType::NoneButton;
}

void ToonConfirmationScreen::UpdateMousePointerState(bool isVisible)
{
	Window::GetWindow()->ShowOSPointer(isVisible);
	m_IsMousePointerVisible = isVisible;
}

void ToonConfirmationScreen::WakeMouseOnMovement()
{
	Vector2 currentMousePosition = Window::GetMouse()->GetWindowPosition();
	if (currentMousePosition != m_MouseLastPosition) { UpdateMousePointerState(true); }
}