#pragma once

#include <string>
#include "Window.h"
#include "ToonGame.h"
#include "ToonGameWorld.h"
#include "InputManager.h"
#include "Toon2DCoordinates.h"

using namespace NCL;
using namespace CSC8503;

#define KEY_BUTTON_DEFAULT_SIZE 5.0f
#define BACKSPACE_BUTTON_WIDTH  15.0f
#define SPACE_BUTTON_WIDTH		25.0f


class ToonVirtualKeyboard
{
	public:
		enum KeyboardInputType
		{
			AlphaNumeric,
			Alphabetic,
			Numpad,
			IPAddress
		};

	private:
		struct KeyData
		{
			std::string text;
			Coordinates coordinates;
			int			identifier;

			KeyData(std::string txt, Coordinates coord, int identifierValue) : text(txt), coordinates(coord), identifier(identifierValue) {}
		};
		
		struct Index2D
		{
			int row;
			int coloumn;

			bool operator==(Index2D rValue) { return ((row == rValue.row) && (coloumn == rValue.coloumn)); }
		};

		enum ActionKeysIdentifiers
		{
			DeleteActionKey = 26,
			SpaceActionKey  = 27,
			DoneActionKey   = 100,
			BackButton		= 200
		};

		std::string		  m_UserInputText;
		Coordinates		  m_Coordinates;
		KeyboardInputType m_KeyboardInputType;
		Index2D			  m_CurrentSelectedKeyIndex;
		Vector4			  m_FocusColour;
		Vector4			  m_UnfocusColour;
		Vector2			  m_WindowSize;
		bool			  m_IsMousePointerVisible = true;
		Vector2			  m_MouseLastPosition	  = Vector2(0.0f, 0.0f);
		Vector2			  m_MousePositionWithinBounds;

		std::vector<std::vector<KeyData>> m_Keys;

	public:
		bool m_HasUserClickedDoneButton = false;
		bool m_HasUserClickedBackButton = false;

	public:
		ToonVirtualKeyboard(Coordinates coordinates, Vector2 windowSize, KeyboardInputType keyboardInputType = KeyboardInputType::IPAddress);
		std::string GetUserInputText();
		void UpdateAndHandleInputEvents();

	private:
		void CreateKeyboard();
		void InitializeAlphabeticKeyboard();
		void InitializeNumericKeyboard();
		void InitializeAlphaNumericKeyboard();
		void InitializeIPAddressKeyboard();
		void DrawKeyboard();
		void DrawSingleKey(std::string keyText, Coordinates coordinate, Index2D index);
		void UpdateMosePointerState(bool isVisible);
		void WakeMouseOnMovement();
		void UpdateCurrentSelectedKeyPositionUsingKeys(KeyboardKeys key);
		bool IsMouseInsideKeyboardArea(float mouseX, float mouseY);
};