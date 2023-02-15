#include "ToonVirtualKeyboard.h"

ToonVirtualKeyboard::ToonVirtualKeyboard(Coordinates coordinates, KeyboardInputType keyboardInputType) : m_Coordinates(coordinates), m_KeyboardInputType(keyboardInputType), m_UserInputText(""), m_CurrentSelectedKeyIndex(0), m_FocusColour(Debug::GREEN), m_UnfocusColour(Debug::WHITE)
{
	CreateKeyboard();
}

std::string ToonVirtualKeyboard::GetUserInputText()
{
	return m_UserInputText;
}

void ToonVirtualKeyboard::UpdateAndHandleInputEvents()
{

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
	float startX = m_Coordinates.origin.x;
	float startY = m_Coordinates.origin.y;
	for (int i = 'A'; i <= 'Z'; i++)
	{
		if (startX + KEY_BUTTON_DEFAULT_SIZE > m_Coordinates.origin.x + m_Coordinates.size.x)
		{
			startX  = m_Coordinates.origin.x;
			startY += KEY_BUTTON_DEFAULT_SIZE;
		}
		std::string keyString		= "";
		keyString				   += i;
		Coordinates keyCoordinates  = Coordinates(Vector2(startX, startY), Vector2(KEY_BUTTON_DEFAULT_SIZE, KEY_BUTTON_DEFAULT_SIZE));
		keys.push_back(KeyData(keyString, keyCoordinates));
		startX					   += KEY_BUTTON_DEFAULT_SIZE;
	}
	if (startX + BACKSPACE_BUTTON_WIDTH > m_Coordinates.origin.x + m_Coordinates.size.x)
	{
		startX  = m_Coordinates.origin.x;
		startY += KEY_BUTTON_DEFAULT_SIZE;
	}
	std::string keyString	   = "Back";
	Coordinates keyCoordinates = Coordinates(Vector2(startX, startY), Vector2(BACKSPACE_BUTTON_WIDTH, KEY_BUTTON_DEFAULT_SIZE));
	keys.push_back(KeyData(keyString, keyCoordinates));
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

	
}

void ToonVirtualKeyboard::DrawSingleKey(std::string keyText, Coordinates coordinate, int index)
{
	Vector4		   keyColour		 = index == m_CurrentSelectedKeyIndex ? m_FocusColour : m_UnfocusColour;
	Debug::DrawQuad(coordinate.origin, coordinate.size, m_UnfocusColour);
	const float	   offset			 = 1.0f;
	const Vector2& characterPosition = Vector2(coordinate.origin.x + (coordinate.size.x / 2) - offset, coordinate.origin.y + (coordinate.size.y / 2) + offset);
	Debug::Print(keyText, characterPosition, keyColour);
}
