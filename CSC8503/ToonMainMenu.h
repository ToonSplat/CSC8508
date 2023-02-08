#pragma once

#include "Window.h"
#include "PushdownMachine.h"
#include "PushdownState.h"
#include "ToonGame.h"

using namespace NCL;
using namespace CSC8503;

class ToonMainMenu : public PushdownState
{
	struct MenuCoordinates
	{
		Vector2 position;
		Vector2 size;
	};
	struct MenuDataStruct
	{
		std::string		text;
		MenuCoordinates coordinates;
		bool			isHovered;
		bool			isSelected;
		Vector4			colour;
	};

	private:
		Window*						m_Window;
		Camera*						m_Camera;
		GameTechRenderer*			m_Renderer;
		ToonGame*					m_Game		       = NULL;
		const Vector4				m_HoverColour	   = Debug::GREEN;
		const Vector4				m_NormalTextColour = Debug::BLUE;
		std::vector<MenuDataStruct> m_mainMenuData = {
														MenuDataStruct("Play", MenuCoordinates(Vector2(5, 10), Vector2(80, 20)), false, false, m_NormalTextColour),
														MenuDataStruct("Multi Play", MenuCoordinates(Vector2(5, 30), Vector2(80, 20)), false, false, m_NormalTextColour),
														MenuDataStruct("Settings", MenuCoordinates(Vector2(5, 50), Vector2(80, 20)), false, false, m_NormalTextColour),
														MenuDataStruct("Credits", MenuCoordinates(Vector2(5, 70), Vector2(80, 20)), false, false, m_NormalTextColour),
														MenuDataStruct("Quit", MenuCoordinates(Vector2(5, 90), Vector2(80, 20)), false, false, m_NormalTextColour)
													 };


	public:
		ToonMainMenu(Window* window);

	private:
		PushdownResult OnUpdate(float dt, PushdownState** newState) override;
		void OnAwake() override;
		void DrawMainMenu();
		bool IsInside(Vector2 mouseCoordinates, MenuCoordinates singleMenuCoordinates);
};