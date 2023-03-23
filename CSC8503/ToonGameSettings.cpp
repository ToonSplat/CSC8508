#include "ToonGameSettings.h"
#include "ToonSettingsManager.h"

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
		if (data.hasToggle)
		{
			data.toggleButton->UpdateButtonDraw(); 
			data.toggleButton->m_IsActive = m_CurrentSelectedIndex == index;
		}
		else if (data.type == ToonGameSettings::SettingsScreenStates::VolumeSlider)
		{
			data.volumeSlider->Update(0.1f);
		}
		index++;
	}
}

void ToonGameSettings::UpdateCurrentSelectedIndex(int incrementBy)
{
	m_CurrentSelectedIndex += incrementBy;
	m_CurrentSelectedIndex  = m_CurrentSelectedIndex < 0 ? (int)m_SettingsData.size() - 1 : m_CurrentSelectedIndex;
	m_CurrentSelectedIndex  = m_CurrentSelectedIndex > m_SettingsData.size() - 1 ? 0 : m_CurrentSelectedIndex;
}

void ToonGameSettings::HandleKeyboardAndMouseEvents()
{
	if (InputManager::GetInstance().GetInputs()[1]->IsPushingDown() || InputManager::GetInstance().GetInputs()[1]->IsPushingUp()) { UpdateMosePointerState(false); }

	if (m_CurrentSelectedIndex == SettingsScreenStates::VolumeSlider) { m_SettingsData[ToonGameSettings::SettingsScreenStates::VolumeSlider].volumeSlider->HandleKeyboardAndMouseEvents(); }

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
	if (InputManager::GetInstance().GetInputs()[1]->IsShootingOnce() || InputManager::GetInstance().GetInputs()[1]->IsSelecting())
	{
		switch (m_CurrentSelectedIndex)
		{
			case SettingsScreenStates::SettingsBack:
				UpdateSettingsFile();
				ToonSettingsManager::ApplySettings();
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
						SettingsDataStructure(Coordinates(Vector2(5.0f, 15.0f), Vector2(80.0f, 10.0f)), m_Window->GetWindow()->GetScreenSize(), "Invert Camera", ToonGameSettings::SettingsScreenStates::InvertCamera, true, InvertCamera, m_SettingsDS.invertCameraState),
						SettingsDataStructure(Coordinates(Vector2(5.0f, 25.0f), Vector2(80.0f, 10.0f)), m_Window->GetWindow()->GetScreenSize(), "Shadow Quality", ToonGameSettings::SettingsScreenStates::Shadow, true, Shadow, m_SettingsDS.shadowState, {"LOW", "HIGH"}),
						SettingsDataStructure(Coordinates(Vector2(5.0f, 35.0f), Vector2(80.0f, 10.0f)), m_Window->GetWindow()->GetScreenSize(), "Resize Window", ToonGameSettings::SettingsScreenStates::WindowSize,  false),
						SettingsDataStructure(Coordinates(Vector2(5.0f, 45.0f), Vector2(80.0f, 10.0f)), m_Window->GetWindow()->GetScreenSize(), "Dynamic Crosshair", ToonGameSettings::SettingsScreenStates::Crosshair, true, Crosshair, m_SettingsDS.crosshairState),
						SettingsDataStructure(Coordinates(Vector2(5.0f, 55.0f), Vector2(80.0f, 10.0f)), m_Window->GetWindow()->GetScreenSize(), "Vsync", ToonGameSettings::SettingsScreenStates::Vsync, true, Vsync, m_SettingsDS.vSyncState),
						SettingsDataStructure(Coordinates(Vector2(5.0f, 65.0f), Vector2(80.0f, 10.0f)), m_Window->GetWindow()->GetScreenSize(), "Aim Trajectory", ToonGameSettings::SettingsScreenStates::AimTrajectory, true, AimTrajectory, m_SettingsDS.aimTrajectoryState),
						SettingsDataStructure(Coordinates(Vector2(5.0f, 75.0f), Vector2(80.0f, 10.0f)), m_Window->GetWindow()->GetScreenSize(), "Volume", ToonGameSettings::SettingsScreenStates::VolumeSlider,  m_SettingsDS.volume),
						SettingsDataStructure(Coordinates(Vector2(5.0f, 85.0f), Vector2(80.0f, 10.0f)), m_Window->GetWindow()->GetScreenSize(), "Back", ToonGameSettings::SettingsScreenStates::SettingsBack, false)
					 };
}

void ToonGameSettings::UpdateSettingsFile()
{
	for (ToonGameSettings::SettingsDataStructure data : m_SettingsData)
	{
		if (data.hasToggle)
		{
			switch (data.toggleButton->m_ToggleButtonID)
			{
				case InvertCamera:
					m_SettingsDS.invertCameraState = data.toggleButton->GetButtonState();
					break;
				case Shadow:
					m_SettingsDS.shadowState = data.toggleButton->GetButtonState();
					break;
				case Crosshair:
					m_SettingsDS.crosshairState = data.toggleButton->GetButtonState();
					break;
				case Vsync:
					m_SettingsDS.vSyncState = data.toggleButton->GetButtonState();
					break;
				case AimTrajectory:
					m_SettingsDS.aimTrajectoryState = data.toggleButton->GetButtonState();
					break;
			}
		}
		if (data.volumeSlider)
		{
			m_SettingsDS.volume = GetStringFromInt(data.volumeSlider->GetCurrentVolumeLevel());
		}
	}
	m_SettingsFile->WriteData((char*)m_SettingsDS.SerializeStructure().c_str(), std::ios_base::trunc);
}

std::string ToonGameSettings::GetStringFromInt(int number)
{
	if (!number) { return "0"; }
	std::string numberString = "";
	int endIndex				 = -1;
	while (number)
	{
		int currDigit  = number % 10;
		numberString  += ('0' + currDigit);
		number		  /= 10;
		endIndex++;
	}
	int startIndex = 0;
	while (startIndex < endIndex)
	{
		char digit				 = numberString[startIndex];
		numberString[startIndex] = numberString[endIndex];
		numberString[endIndex]	 = digit;
		startIndex++;
		endIndex--;
	}
	return numberString;
}
