#include "ToonVirtualKeyboard.h"

ToonVirtualKeyboard::ToonVirtualKeyboard(Coordinates coordinates, Vector2 windowSize, KeyboardInputType keyboardInputType) : m_Coordinates(coordinates), m_WindowSize(windowSize), m_KeyboardInputType(keyboardInputType), m_UserInputText(""), m_CurrentSelectedKeyIndex(0), m_FocusColour(Debug::GREEN), m_UnfocusColour(Debug::WHITE)
{
	CreateKeyboard();
}

std::string ToonVirtualKeyboard::GetUserInputText()
{
	return m_UserInputText;
}

void ToonVirtualKeyboard::UpdateAndHandleInputEvents()
{
	DrawKeyboard();
	if (m_IsMousePointerVisible && Window::GetMouse()->ButtonPressed(MouseButtons::LEFT) || Window::GetKeyboard()->KeyPressed(KeyboardKeys::RETURN))
	{
		if (keys[m_CurrentSelectedKeyIndex].identifier == 26)
		{
			if (m_UserInputText.length()) { m_UserInputText.pop_back(); }
		}
		else { m_UserInputText += keys[m_CurrentSelectedKeyIndex].text; }
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::LEFT))	{ UpdateCurrentSelectedKeyPositionUsingKeys(KeyboardKeys::LEFT); }
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::RIGHT)) { UpdateCurrentSelectedKeyPositionUsingKeys(KeyboardKeys::RIGHT); }
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::UP))	{ UpdateCurrentSelectedKeyPositionUsingKeys(KeyboardKeys::UP); }
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::DOWN))	{ UpdateCurrentSelectedKeyPositionUsingKeys(KeyboardKeys::DOWN); }
	if (!m_IsMousePointerVisible)								{ WakeMouseOnMovement(); }
}

void ToonVirtualKeyboard::CreateKeyboard()
{
	switch (m_KeyboardInputType)
	{
		case KeyboardInputType::Alphabetic:
			InitializeAlphabeticKeyboard();
			break;
		case KeyboardInputType::AlphaNumeric:
			InitializeAlphaNumericKeyboard();
			break;
		case KeyboardInputType::IPAddress:
			InitializeIPAddressKeyboard();
			break;
		case KeyboardInputType::Numpad:
			InitializeNumericKeyboard();
			break;
	}
}

void ToonVirtualKeyboard::InitializeAlphabeticKeyboard()
{
	const float padding = 0.5f;
	float		startX  = m_Coordinates.origin.x;
	float		startY  = m_Coordinates.origin.y + padding;
	for (int i = 'A'; i <= 'Z'; i++)
	{
		if (startX + KEY_BUTTON_DEFAULT_SIZE > m_Coordinates.origin.x + m_Coordinates.size.x)
		{
			startX  = m_Coordinates.origin.x;
			startY += KEY_BUTTON_DEFAULT_SIZE + padding;
		}
		std::string keyString		= "";
		keyString				   += i;
		Coordinates keyCoordinates  = Coordinates(Vector2(startX, startY), Vector2(KEY_BUTTON_DEFAULT_SIZE, KEY_BUTTON_DEFAULT_SIZE));
		keys.push_back(KeyData(keyString, keyCoordinates, i - 'A'));
		startX					   += KEY_BUTTON_DEFAULT_SIZE + padding;
	}
	if (startX + BACKSPACE_BUTTON_WIDTH > m_Coordinates.origin.x + m_Coordinates.size.x)
	{
		startX  = m_Coordinates.origin.x;
		startY += KEY_BUTTON_DEFAULT_SIZE;
	}
	m_Coordinates.size.y	   = startY + KEY_BUTTON_DEFAULT_SIZE;
	std::string keyString	   = "Back";
	Coordinates keyCoordinates = Coordinates(Vector2(startX, startY), Vector2(BACKSPACE_BUTTON_WIDTH, KEY_BUTTON_DEFAULT_SIZE));
	keys.push_back(KeyData(keyString, keyCoordinates, 26));
}

void ToonVirtualKeyboard::InitializeNumericKeyboard()
{
}

void ToonVirtualKeyboard::InitializeAlphaNumericKeyboard()
{
}

void ToonVirtualKeyboard::InitializeIPAddressKeyboard()
{
}

void ToonVirtualKeyboard::DrawKeyboard()
{
	Vector2 mousePosition			  = Window::GetMouse()->GetWindowPosition();
	float	y						  = ((mousePosition.y / m_WindowSize.y) * 100);
	float	x						  = ((mousePosition.x / m_WindowSize.x) * 100);
	Vector2 mousePositionWithinBounds = Vector2(x, y);

	unsigned int index = 0;
	for (KeyData key : keys)
	{
		if (m_IsMousePointerVisible && x >= m_Coordinates.origin.x && x <= m_Coordinates.origin.x + m_Coordinates.size.x && y >= m_Coordinates.origin.y && y <= m_Coordinates.origin.y + m_Coordinates.size.y)
		{
			if (x >= key.coordinates.origin.x && x <= key.coordinates.origin.x + key.coordinates.size.x && y >= key.coordinates.origin.y && y <= key.coordinates.origin.y + key.coordinates.size.y) { m_CurrentSelectedKeyIndex = index; }
		}
		//else { m_CurrentSelectedKeyIndex = 0; }
		DrawSingleKey(key.text, key.coordinates, index);
		index++;
	}
}

void ToonVirtualKeyboard::DrawSingleKey(std::string keyText, Coordinates coordinate, int index)
{
	Vector4		   keyColour		 = index == m_CurrentSelectedKeyIndex ? m_FocusColour : m_UnfocusColour;
	Debug::DrawQuad(coordinate.origin, coordinate.size, keyColour);
	const float	   offset			 = 1.0f;
	const Vector2& characterPosition = Vector2(coordinate.origin.x + (coordinate.size.x / 2) - (offset * (float)keyText.length()), coordinate.origin.y + (coordinate.size.y / 2) + offset);
	Debug::Print(keyText, characterPosition, keyColour);
}

void ToonVirtualKeyboard::UpdateMosePointerState(bool isVisible)
{
	Window::GetWindow()->ShowOSPointer(isVisible);
	m_IsMousePointerVisible = isVisible;
}

void ToonVirtualKeyboard::WakeMouseOnMovement()
{
	Vector2 currentMousePosition = Window::GetMouse()->GetWindowPosition();
	if (currentMousePosition != m_MouseLastPosition) { UpdateMosePointerState(true); }
	m_MouseLastPosition = currentMousePosition;
}

void ToonVirtualKeyboard::UpdateCurrentSelectedKeyPositionUsingKeys(KeyboardKeys key)
{
	m_MouseLastPosition		  = Window::GetMouse()->GetWindowPosition();
	UpdateMosePointerState(false);
	int totalKeys			  = keys.size();
	switch (key)
	{
		case KeyboardKeys::RIGHT:
			m_CurrentSelectedKeyIndex = (m_CurrentSelectedKeyIndex + 1) % totalKeys;
			break;
		case KeyboardKeys::LEFT:
			m_CurrentSelectedKeyIndex = (m_CurrentSelectedKeyIndex - 1) < 0 ? keys.size() - 1 : m_CurrentSelectedKeyIndex - 1;
			break;
		case KeyboardKeys::UP:
			m_CurrentSelectedKeyIndex -= 5;
			m_CurrentSelectedKeyIndex  = m_CurrentSelectedKeyIndex < 0 ? keys.size() - 1 : m_CurrentSelectedKeyIndex;
			break;
		case KeyboardKeys::DOWN:
			m_CurrentSelectedKeyIndex += 5;
			m_CurrentSelectedKeyIndex  = m_CurrentSelectedKeyIndex > keys.size() - 1 ? m_CurrentSelectedKeyIndex - keys.size() : m_CurrentSelectedKeyIndex;
			break;
	}
}