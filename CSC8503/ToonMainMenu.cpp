#include "ToonMainMenu.h"

ToonMainMenu::ToonMainMenu(Window* window)
{
	m_Window			 = window;
	m_Camera			 = new Camera();
	ToonGameWorld* world = new ToonGameWorld();
	m_Renderer			 = new GameTechRenderer(*world);
	Window::GetWindow()->ShowOSPointer(true);
	Window::GetWindow()->LockMouseToWindow(false);
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
	m_Renderer->Render();
	return PushdownResult::NoChange;
}

void ToonMainMenu::OnAwake()
{
	m_Game = new ToonGame();
	std::cout << "Welcome to a really awesome game!\n";
	std::cout << "Press Space To Begin or escape to quit!\n";
}

bool ToonMainMenu::IsInside(Vector2 mouseCoordinates, MenuCoordinates singleMenuCoordinates)
{
	int widthConstraint  = singleMenuCoordinates.position.x + singleMenuCoordinates.size.x;
	int heightConstraint = singleMenuCoordinates.position.y + singleMenuCoordinates.size.y;
	if (mouseCoordinates.x >= singleMenuCoordinates.position.x && mouseCoordinates.x <= widthConstraint && mouseCoordinates.y >= singleMenuCoordinates.position.y && mouseCoordinates.y <= heightConstraint)
	{
		std::cout << "Is Inside" << std::endl;
	}
	return (mouseCoordinates.x >= singleMenuCoordinates.position.x && mouseCoordinates.x <= widthConstraint && mouseCoordinates.y >= singleMenuCoordinates.position.y && mouseCoordinates.y <= heightConstraint);
}

void ToonMainMenu::DrawMainMenu()
{
	Ray r = CollisionDetection::BuildRayFromMouse(*m_Camera);
	Vector2 mousePosition = r.GetPosition();//Window::GetMouse()->GetWindowPosition();
	std::string str = std::to_string(mousePosition.x) + ", " + std::to_string(mousePosition.y);
	std::cout << mousePosition << std::endl;
	Debug::Print(str, Vector2(50, 50), Debug::RED);
	for (auto data : m_mainMenuData)
	{
		data.colour = IsInside(mousePosition, data.coordinates) ? m_HoverColour : m_NormalTextColour;
		Debug::Print(data.text, data.coordinates.position, data.colour);
	}
}