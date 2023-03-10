#include "ToonVirtualKeyboard.h"

ToonVirtualKeyboard::ToonVirtualKeyboard(Coordinates coordinates, Vector2 windowSize, KeyboardInputType keyboardInputType) : m_Coordinates(coordinates), m_WindowSize(windowSize), m_KeyboardInputType(keyboardInputType), m_UserInputText(""), m_CurrentSelectedKeyIndex(Index2D(0, 0)), m_FocusColour(Debug::GREEN), m_UnfocusColour(Debug::WHITE)
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
	if (m_IsMousePointerVisible && IsMouseInsideKeyboardArea(m_MousePositionWithinBounds.x, m_MousePositionWithinBounds.y) && InputManager::GetInstance().GetInputs()[1]->IsShootingOnce() || InputManager::GetInstance().GetInputs()[1]->IsSelecting())
	{
		AudioSystem::GetAudioSystem()->SelectMenuOption();
		if (m_Keys[m_CurrentSelectedKeyIndex.row][m_CurrentSelectedKeyIndex.coloumn].identifier == ActionKeysIdentifiers::DeleteActionKey)
		{
			if (m_UserInputText.length()) { m_UserInputText.pop_back(); }
		}
		else if (m_KeyboardInputType != IPAddress && m_Keys[m_CurrentSelectedKeyIndex.row][m_CurrentSelectedKeyIndex.coloumn].identifier == ActionKeysIdentifiers::SpaceActionKey) { m_UserInputText += " "; }
		else if (m_Keys[m_CurrentSelectedKeyIndex.row][m_CurrentSelectedKeyIndex.coloumn].identifier == ActionKeysIdentifiers::DoneActionKey) { m_HasUserClickedDoneButton = true; }
		else if (m_Keys[m_CurrentSelectedKeyIndex.row][m_CurrentSelectedKeyIndex.coloumn].identifier == ActionKeysIdentifiers::BackButton) { m_HasUserClickedBackButton = true; }
		else { m_UserInputText += m_Keys[m_CurrentSelectedKeyIndex.row][m_CurrentSelectedKeyIndex.coloumn].text; }
	}
	if (m_KeyboardInputType != IPAddress) {  }
	if (InputManager::GetInstance().GetInputs()[1]->IsPushingLeft())	{ UpdateCurrentSelectedKeyPositionUsingKeys(KeyboardKeys::LEFT); }
	if (InputManager::GetInstance().GetInputs()[1]->IsPushingRight())	{ UpdateCurrentSelectedKeyPositionUsingKeys(KeyboardKeys::RIGHT); }
	if (InputManager::GetInstance().GetInputs()[1]->IsPushingUp())		{ UpdateCurrentSelectedKeyPositionUsingKeys(KeyboardKeys::UP); }
	if (InputManager::GetInstance().GetInputs()[1]->IsPushingDown())	{ UpdateCurrentSelectedKeyPositionUsingKeys(KeyboardKeys::DOWN); }
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
	std::vector<KeyData> singleRowKeys;
	for (int i = 'A'; i <= 'Z'; i++)
	{
		if (startX + KEY_BUTTON_DEFAULT_SIZE > m_Coordinates.origin.x + m_Coordinates.size.x)
		{
			m_Keys.push_back(singleRowKeys);
			singleRowKeys = std::vector<KeyData>();
			startX		  = m_Coordinates.origin.x;
			startY		 += KEY_BUTTON_DEFAULT_SIZE + padding;
		}
		std::string keyString		= "";
		keyString				   += i;
		Coordinates keyCoordinates  = Coordinates(Vector2(startX, startY), Vector2(KEY_BUTTON_DEFAULT_SIZE, KEY_BUTTON_DEFAULT_SIZE));
		singleRowKeys.push_back(KeyData(keyString, keyCoordinates, i - 'A'));
		startX					   += KEY_BUTTON_DEFAULT_SIZE + padding;
	}
	if (startX + BACKSPACE_BUTTON_WIDTH > m_Coordinates.origin.x + m_Coordinates.size.x)
	{
		startX  = m_Coordinates.origin.x;
		startY += KEY_BUTTON_DEFAULT_SIZE;
	}
	if (singleRowKeys.size()) { m_Keys.push_back(singleRowKeys); }
	m_Coordinates.size.y	   = startY + KEY_BUTTON_DEFAULT_SIZE;
	std::string keyString	   = "Delete";
	Coordinates keyCoordinates = Coordinates(Vector2(startX, startY), Vector2(BACKSPACE_BUTTON_WIDTH, KEY_BUTTON_DEFAULT_SIZE));
	m_Keys[m_Keys.size() - 1].push_back(KeyData(keyString, keyCoordinates, ActionKeysIdentifiers::DeleteActionKey));
	//Adding Space bar
	startY					  += KEY_BUTTON_DEFAULT_SIZE + padding;
	startX					   = m_Coordinates.origin.x;
	singleRowKeys			   = std::vector<KeyData>();
	m_Coordinates.size.y	   = startY + KEY_BUTTON_DEFAULT_SIZE;
	std::string spaceKeyString = "Space";
	keyCoordinates			   = Coordinates(Vector2(startX, startY), Vector2(SPACE_BUTTON_WIDTH, KEY_BUTTON_DEFAULT_SIZE));
	singleRowKeys.push_back(KeyData(spaceKeyString, keyCoordinates, ActionKeysIdentifiers::SpaceActionKey));
	m_Keys.push_back(singleRowKeys);
}

void ToonVirtualKeyboard::InitializeNumericKeyboard()
{
}

void ToonVirtualKeyboard::InitializeAlphaNumericKeyboard()
{
}

void ToonVirtualKeyboard::InitializeIPAddressKeyboard()
{
	const int			 keysInARow = 3;
	const float			 padding    = 0.5f;
	const float			 initialX	= m_Coordinates.origin.x + ((m_Coordinates.size.x - (keysInARow * KEY_BUTTON_DEFAULT_SIZE) - ((keysInARow - 1) * padding)) / 2);
	float				 startX	    = initialX;
	float				 startY	    = m_Coordinates.origin.y + padding;
	std::vector<KeyData> singleRowsKeys;
	for (int i = 0; i < 9; i++)
	{
		if (i && !(i % keysInARow))
		{
			m_Keys.push_back(singleRowsKeys);
			singleRowsKeys = std::vector<KeyData>();
			startX		   = initialX;
			startY		  += KEY_BUTTON_DEFAULT_SIZE + padding;
		}
		std::string text = "";
		text			+= ((i + 1) + '0');
		singleRowsKeys.push_back(KeyData(text, Coordinates(Vector2(startX, startY), Vector2(KEY_BUTTON_DEFAULT_SIZE, KEY_BUTTON_DEFAULT_SIZE)), i + 1));
		startX			+= KEY_BUTTON_DEFAULT_SIZE + padding;
	}
	if (!singleRowsKeys.empty()) { m_Keys.push_back(singleRowsKeys); }
	//Adding 0
	startY		  += KEY_BUTTON_DEFAULT_SIZE + padding;
	startX		   = initialX;
	singleRowsKeys = std::vector<KeyData>();
	singleRowsKeys.push_back(KeyData("0", Coordinates(Vector2(startX, startY), Vector2(KEY_BUTTON_DEFAULT_SIZE, KEY_BUTTON_DEFAULT_SIZE)), 0));
	// Adding .
	startX += KEY_BUTTON_DEFAULT_SIZE + padding;
	singleRowsKeys.push_back(KeyData(".", Coordinates(Vector2(startX, startY), Vector2(KEY_BUTTON_DEFAULT_SIZE, KEY_BUTTON_DEFAULT_SIZE)), 10));
	//Delete button
	startX					  += KEY_BUTTON_DEFAULT_SIZE + padding;
	Coordinates keyCoordinates = Coordinates(Vector2(startX, startY), Vector2(KEY_BUTTON_DEFAULT_SIZE, KEY_BUTTON_DEFAULT_SIZE));
	singleRowsKeys.push_back(KeyData("<-", keyCoordinates, ActionKeysIdentifiers::DeleteActionKey));
	m_Keys.push_back(singleRowsKeys);
	//Adding Done Button
	startY		  += KEY_BUTTON_DEFAULT_SIZE + padding;
	startX		   = initialX;
	singleRowsKeys = std::vector<KeyData>();
	singleRowsKeys.push_back(KeyData("Done", Coordinates(Vector2(startX, startY), Vector2(BACKSPACE_BUTTON_WIDTH, KEY_BUTTON_DEFAULT_SIZE)), ActionKeysIdentifiers::DoneActionKey));
	m_Keys.push_back(singleRowsKeys);
	m_Coordinates.size.y = startY + KEY_BUTTON_DEFAULT_SIZE - m_Coordinates.origin.y;
	//Adding Back Button
	startY += (2 * KEY_BUTTON_DEFAULT_SIZE);
	startX  = initialX;
	singleRowsKeys = std::vector<KeyData>();
	singleRowsKeys.push_back(KeyData("BACK", Coordinates(Vector2(startX, startY), Vector2(BACKSPACE_BUTTON_WIDTH, KEY_BUTTON_DEFAULT_SIZE)), ActionKeysIdentifiers::BackButton));
	m_Keys.push_back(singleRowsKeys);
	m_Coordinates.size.y = startY + (2 * KEY_BUTTON_DEFAULT_SIZE) - m_Coordinates.origin.y;
}

void ToonVirtualKeyboard::DrawKeyboard()
{
	Vector2 mousePosition		= InputManager::GetInstance().GetInputs()[1]->GetMousePosition();
	float	y					= ((mousePosition.y / m_WindowSize.y) * 100);
	float	x					= ((mousePosition.x / m_WindowSize.x) * 100);
	m_MousePositionWithinBounds	= Vector2(x, y);

	unsigned int rowIndex = 0;
	for (std::vector<KeyData> keysRow : m_Keys)
	{
		unsigned int ColoumnIndex = 0;
		for (KeyData key : keysRow)
		{
			if (m_IsMousePointerVisible && IsMouseInsideKeyboardArea(x, y))
			{
				if (x >= key.coordinates.origin.x && x <= key.coordinates.origin.x + key.coordinates.size.x && y >= key.coordinates.origin.y && y <= key.coordinates.origin.y + key.coordinates.size.y) { 
					Index2D lastIndex = m_CurrentSelectedKeyIndex;
					m_CurrentSelectedKeyIndex = Index2D(rowIndex, ColoumnIndex); 
					if(lastIndex != m_CurrentSelectedKeyIndex) AudioSystem::GetAudioSystem()->SelectMenuOption();
				}
			}
			//else { m_CurrentSelectedKeyIndex = 0; }
			DrawSingleKey(key.text, key.coordinates, Index2D(rowIndex, ColoumnIndex));
			ColoumnIndex++;
		}
		rowIndex++;
	}
}

void ToonVirtualKeyboard::DrawSingleKey(std::string keyText, Coordinates coordinate, Index2D index)
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
	Vector2 currentMousePosition = InputManager::GetInstance().GetInputs()[1]->GetMousePosition();
	if (currentMousePosition != m_MouseLastPosition) { UpdateMosePointerState(true); }
	m_MouseLastPosition = currentMousePosition;
}

void ToonVirtualKeyboard::UpdateCurrentSelectedKeyPositionUsingKeys(KeyboardKeys key)
{
	AudioSystem::GetAudioSystem()->SelectMenuOption();
	m_MouseLastPosition = InputManager::GetInstance().GetInputs()[1]->GetMousePosition();
	UpdateMosePointerState(false);
	int totalKeys		= (int)m_Keys.size();
	switch (key)
	{
		case KeyboardKeys::RIGHT:
			m_CurrentSelectedKeyIndex.coloumn = (m_CurrentSelectedKeyIndex.coloumn + 1) % m_Keys[m_CurrentSelectedKeyIndex.row].size();
			break;
		case KeyboardKeys::LEFT:
			m_CurrentSelectedKeyIndex.coloumn = (m_CurrentSelectedKeyIndex.coloumn - 1) < 0 ? (int)m_Keys[m_CurrentSelectedKeyIndex.row].size() - 1 : m_CurrentSelectedKeyIndex.coloumn - 1;
			break;
		case KeyboardKeys::UP:
			m_CurrentSelectedKeyIndex.row	 -= 1;
			m_CurrentSelectedKeyIndex.row	  = m_CurrentSelectedKeyIndex.row < 0 ? (int)m_Keys.size() - 1 : m_CurrentSelectedKeyIndex.row;
			m_CurrentSelectedKeyIndex.coloumn = m_CurrentSelectedKeyIndex.coloumn >= (int)m_Keys[m_CurrentSelectedKeyIndex.row].size() ? (int)m_Keys[m_CurrentSelectedKeyIndex.row].size() - 1 : m_CurrentSelectedKeyIndex.coloumn;
			break;
		case KeyboardKeys::DOWN:
			m_CurrentSelectedKeyIndex.row	 += 1;
			m_CurrentSelectedKeyIndex.row	  = m_CurrentSelectedKeyIndex.row > m_Keys.size() - 1 ? 0 : m_CurrentSelectedKeyIndex.row;
			m_CurrentSelectedKeyIndex.coloumn = m_CurrentSelectedKeyIndex.coloumn >= m_Keys[m_CurrentSelectedKeyIndex.row].size() ? (int)m_Keys[m_CurrentSelectedKeyIndex.row].size() - 1 : m_CurrentSelectedKeyIndex.coloumn;
			break;
	}
}

bool ToonVirtualKeyboard::IsMouseInsideKeyboardArea(float mouseX, float mouseY)
{
	return (mouseX >= m_Coordinates.origin.x && mouseX <= m_Coordinates.origin.x + m_Coordinates.size.x && mouseY >= m_Coordinates.origin.y && mouseY <= m_Coordinates.origin.y + m_Coordinates.size.y);
}