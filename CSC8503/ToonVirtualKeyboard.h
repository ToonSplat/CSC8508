#pragma once

#include <string>
#include "Window.h"
#include "ToonGame.h"
#include "ToonGameWorld.h"

using namespace NCL;
using namespace CSC8503;

#define KEY_BUTTON_DEFAULT_SIZE 5.0f
#define BACKSPACE_BUTTON_WIDTH  8.0f

//TODO: Use this in Main Menu screen
struct Coordinates
{
	Vector2 origin;
	Vector2 size;

	Coordinates(Vector2 org, Vector2 siz) : origin(org), size(siz) {}
};

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
		std::string		  m_UserInputText;
		Coordinates		  m_Coordinates;
		KeyboardInputType m_KeyboardInputType;
		int				  m_CurrentSelectedKeyIndex;
		Vector4			  m_FocusColour;
		Vector4			  m_UnfocusColour;

		struct KeyData
		{
			std::string text;
			Coordinates coordinates;

			KeyData(std::string txt, Coordinates coord) : text(txt), coordinates(coord) {}
		};

		std::vector<KeyData> keys;

	public:
		ToonVirtualKeyboard(Coordinates coordinates, KeyboardInputType keyboardInputType = KeyboardInputType::IPAddress);
		std::string GetUserInputText();
		void UpdateAndHandleInputEvents();

	private:
		void CreateKeyboard();
		void InitializeAlphabeticKeyboard();
		void InitializeNumericKeyboard();
		void InitializeAlphaNumericKeyboard();
		void InitializeIPAddressKeyboard();
		void DrawKeyboard();
		void DrawSingleKey(std::string keyText, Coordinates coordinate, int index);
};