#include "ToonGameSettings.h"

ToonGameSettings::ToonGameSettings(GameTechRenderer* renderer, ToonGameWorld* world, Window* win)
{
	m_Renderer = renderer;
	m_Window   = win;
	m_World	   = world;
	m_InvertCameraToggleButton = new ToonToggleButton(Coordinates(Vector2(50.0f, 40.0f), Vector2(10.0f, 10.0f)), true);
}

ToonGameSettings::~ToonGameSettings()
{
	delete m_InvertCameraToggleButton;
	m_InvertCameraToggleButton = NULL;
}

PushdownState::PushdownResult ToonGameSettings::OnUpdate(float dt, PushdownState** newState)
{
	m_Renderer->Update(dt);
	m_Renderer->Render();
	Debug::UpdateRenderables(dt);
	DrawScreen();
	return PushdownResult::NoChange;
}

void ToonGameSettings::OnAwake()
{
}

void ToonGameSettings::OnSleep()
{
}

void ToonGameSettings::DrawScreen()
{
	Debug::Print("Test Settings screen", Vector2(20.0f, 20.0f), Debug::WHITE);
	m_InvertCameraToggleButton->UpdateButtonDraw();
}