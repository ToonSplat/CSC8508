#pragma once

#include "Window.h"
#include "PushdownMachine.h"
#include "PushdownState.h"
#include "ToonGame.h"

using namespace NCL;
using namespace CSC8503;

class ToonMainMenu : public PushdownState
{
	struct MenuDataStruct
	{
		std::string text;
		Vector2		xyLocation;
		bool		isHovered;
		bool		isSelected;
		Vector4		colour;
	};

	private:
		Window*						m_Window;
		ToonGame*					m_Game		   = NULL;
		std::vector<MenuDataStruct> m_mainMenuData = {
														MenuDataStruct("Play", Vector2(5, 10), false, false, m_NormalTextColour),
														MenuDataStruct("Multi Play", Vector2(5, 30), false, false, m_NormalTextColour),
														MenuDataStruct("Settings", Vector2(5, 50), false, false, m_NormalTextColour),
														MenuDataStruct("Credits", Vector2(5, 70), false, false, m_NormalTextColour),
														MenuDataStruct("Quit", Vector2(5, 90), false, false, m_NormalTextColour)
													 };

		Vector4				m_HoverColour		   = Debug::GREEN;
		Vector4				m_NormalTextColour	   = Debug::BLUE;

	public:
		ToonMainMenu(Window* window);

	private:
		PushdownResult OnUpdate(float dt, PushdownState** newState) override;
		void OnAwake() override;
		void DrawMainMenu();
};