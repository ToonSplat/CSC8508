#include "ToonGameSettings.h"

ToonGameSettings::ToonGameSettings(GameTechRenderer* renderer, ToonGameWorld* world, Window* win)
{
	m_Renderer = renderer;
	m_Window   = win;
	m_World	   = world;
	PopulateSettingsData();
}

ToonGameSettings::~ToonGameSettings()
{
	FreeAllToonToggleButtons();
}

PushdownState::PushdownResult ToonGameSettings::OnUpdate(float dt, PushdownState** newState)
{
	m_Renderer->Update(dt);
	m_Renderer->Render();
	Debug::UpdateRenderables(dt);
	if (!m_IsMousePointerVisible) { WakeMouseOnMovement(); }
	HandleKeyboardAndMouseEvents();
	DrawScreen();
	return HandleNavigation(newState);
}

void ToonGameSettings::OnAwake()
{
}

void ToonGameSettings::OnSleep()
{
}

void ToonGameSettings::DrawScreen()
{
	int index = 0;
	for (SettingsDataStructure data : m_SettingsData)
	{
		Debug::Print(data.text, data.coordinates.origin, index == m_CurrentSelectedIndex ? m_SelectedColour : m_NonSelectedColour);
		if (data.hasToggle) { data.toggleButton->UpdateButtonDraw(); }
		data.toggleButton->m_IsActive = m_CurrentSelectedIndex == index;
		index++;
	}
}

void ToonGameSettings::UpdateCurrentSelectedIndex(int incrementBy)
{
	m_CurrentSelectedIndex += incrementBy;
	m_CurrentSelectedIndex  = m_CurrentSelectedIndex < 0 ? m_SettingsData.size() - 1 : m_CurrentSelectedIndex;
	m_CurrentSelectedIndex  = m_CurrentSelectedIndex > m_SettingsData.size() - 1 ? 0 : m_CurrentSelectedIndex;
}

void ToonGameSettings::HandleKeyboardAndMouseEvents()
{
	if (InputManager::GetInstance().GetInputs()[1]->IsPushingDown() || InputManager::GetInstance().GetInputs()[1]->IsPushingUp()) { UpdateMosePointerState(false); }

	if (!m_IsMousePointerVisible)
	{
		int increment = 0;
		if		(InputManager::GetInstance().GetInputs()[1]->IsPushingDown()) { increment =  1; }
		else if (InputManager::GetInstance().GetInputs()[1]->IsPushingUp())   { increment = -1; }
		UpdateCurrentSelectedIndex(increment);
	}
	else
	{
		Vector2 mousePosition			  = InputManager::GetInstance().GetInputs()[1]->GetMousePosition();
		Vector2 windowSize				  = m_Window->GetWindow()->GetScreenSize();
		float	y						  = ((mousePosition.y / windowSize.y) * 100) + 5.0f;
		float	x						  = ((mousePosition.x / windowSize.x) * 100) + 5.0f;
		Vector2 mousePositionWithinBounds = Vector2(x, y);
		int index = 0;
		for (SettingsDataStructure data : m_SettingsData)
		{
			if (isInside(mousePositionWithinBounds, data.coordinates))
			{
				m_CurrentSelectedIndex = index;
			}
			index++;
		}
	}
}

void ToonGameSettings::UpdateMosePointerState(bool isVisible)
{
	Window::GetWindow()->ShowOSPointer(isVisible);
	m_IsMousePointerVisible = isVisible;
}

void ToonGameSettings::WakeMouseOnMovement()
{
	Vector2 currentMousePosition = InputManager::GetInstance().GetInputs()[1]->GetMousePosition();
	if (currentMousePosition != m_MouseLastPosition) { UpdateMosePointerState(true); }
	m_MouseLastPosition = currentMousePosition;
}

bool ToonGameSettings::isInside(Vector2 mousePosition, Coordinates menuDataCoordinates)
{
	float widthConstraint  = menuDataCoordinates.origin.x + menuDataCoordinates.size.x;
	float heightConstraint = menuDataCoordinates.origin.y + menuDataCoordinates.size.y;
	return (mousePosition.x >= menuDataCoordinates.origin.x && mousePosition.x <= widthConstraint && mousePosition.y >= menuDataCoordinates.origin.y && mousePosition.y <= heightConstraint);
}

PushdownState::PushdownResult ToonGameSettings::HandleNavigation(PushdownState** newState)
{
	if (InputManager::GetInstance().GetInputs()[1]->IsShooting() || InputManager::GetInstance().GetInputs()[1]->IsSelecting())
	{
		switch (m_CurrentSelectedIndex)
		{
			case SettingsScreenStates::SettingsBack:
				UpdateSettingsFile();
				return PushdownState::PushdownResult::Pop;
			case SettingsScreenStates::WindowSize:
				break;
		}
	}
	return PushdownState::PushdownResult::NoChange;
}

void ToonGameSettings::FreeAllToonToggleButtons()
{
	for (SettingsDataStructure data : m_SettingsData)
	{
		if (data.toggleButton)
		{
			delete(data.toggleButton);
			data.toggleButton = NULL;
		}
	}
}

void ToonGameSettings::PopulateSettingsData()
{
	m_SettingsFile			= new ToonFileHandling(Settings_File_Name);
	std::string fileContent = "";
	m_SettingsFile->ReadDataIn(fileContent);

	m_SettingsDS.ParseData(fileContent);

	m_SettingsData = {
						SettingsDataStructure(Coordinates(Vector2(5.0f, 20.0f), Vector2(80.0f, 10.0f)), "Invert Camera", true, m_SettingsDS.invertCameraState),
						SettingsDataStructure(Coordinates(Vector2(5.0f, 30.0f), Vector2(80.0f, 10.0f)), "Enable Shadow", true, m_SettingsDS.shadowState),
						SettingsDataStructure(Coordinates(Vector2(5.0f, 40.0f), Vector2(80.0f, 10.0f)), "Resize Window", false),
						SettingsDataStructure(Coordinates(Vector2(5.0f, 50.0f), Vector2(80.0f, 10.0f)), "Back",		     false)
					 };
}

void ToonGameSettings::UpdateSettingsFile()
{
	m_SettingsFile->WriteData(m_SettingsDS.SerializeStructure());
}