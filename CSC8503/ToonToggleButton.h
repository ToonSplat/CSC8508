#pragma once
//#include "Debug.h"
//#include "Toon2DCoordinates.h"


#include "GameTechRenderer.h"
#include "Toon2DCoordinates.h"
#include "InputManager.h"

using namespace NCL;
using namespace CSC8503;


enum ToggleButtonStates
{
	ToggleOff,
	ToggleOn
};

class ToonToggleButton
{
	private:
		Coordinates		   m_Coordinates;
		ToggleButtonStates m_CurrentState;
		const Vector4	   m_ToggleOnColour  = Debug::GREEN;
		const Vector4	   m_ToggleOffColour = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		const Vector4	   m_HeadColour		 = Debug::WHITE;
		Coordinates		   m_HalfSize;
		bool			   m_ShouldDrawValue = false;

	public:
		ToonToggleButton(Coordinates coordinates, bool shouldDrawValue = false);
		~ToonToggleButton();
		ToggleButtonStates GetButtonState();
		void UpdateButtonDraw();
		void HandleEvents();
	
	private:
		void DrawButton();
		void DrawOffToggle();
		void DrawOnToggle();
		void DrawHead();
		void ToggleState();
		void DrawValue();
};