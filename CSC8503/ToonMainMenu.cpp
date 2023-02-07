#include "ToonMainMenu.h"

ToonMainMenu::ToonMainMenu(Window* window)
{
	m_Window = window;
}

PushdownState::PushdownResult ToonMainMenu::OnUpdate(float dt, PushdownState** newState)
{
	DrawMainMenu();

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE))
	{
		*newState = m_Game;
		return PushdownResult::Push;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE))
		return PushdownResult::Pop;

	return PushdownResult::NoChange;
}

void ToonMainMenu::OnAwake()
{
	m_Game = new ToonGame();
	std::cout << "Welcome to a really awesome game!\n";
	std::cout << "Press Space To Begin or escape to quit!\n";
}

void ToonMainMenu::DrawMainMenu()
{
	for (auto data : m_mainMenuData)
	{
		Debug::Print(data.text, data.xyLocation, data.colour);
	}
}