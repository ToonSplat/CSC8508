#include "ToonMainMenu.h"
#include <regex>

ToonMainMenu::ToonMainMenu(GameTechRenderer* renderer, ToonGameWorld* world, Window* win)
{
	m_Renderer = renderer;
	m_World = world;
	m_CurrentSelectedIndex = 0;
	m_Window = win;

	if (!mainMenuTune)
		ToonMainMenuAudioInitialise();
	m_ToonConfirmationScreen = new ToonConfirmationScreen(Coordinates(Vector2(30, 20), Vector2(50, 20)), m_Window->GetScreenSize(), m_Renderer);
	m_ToonConfirmationScreen->delegate = this;
}

ToonMainMenu::ToonMainMenu(GameTechRenderer* renderer, std::vector<MenuDataStruct> menuData, int baseCurrentSelectedIndex, ToonGameWorld* world, Window* win, AudioEmitter* e)
{
	m_Renderer = renderer;
	m_mainMenuData = menuData;
	m_BaseCurrentSelectdIndex = baseCurrentSelectedIndex;
	m_CurrentSelectedIndex = 0;
	m_Window = win;
	m_World = world;
  m_ToonConfirmationScreen = new ToonConfirmationScreen(Coordinates(Vector2(30, 20), Vector2(20, 20)), m_Window->GetScreenSize(), m_Renderer);
	m_ToonConfirmationScreen->delegate = this;

	optionClick = e;
}

ToonMainMenu::~ToonMainMenu()
{
if (gameTune)
		delete gameTune;
	if (mainMenuTune)
		delete mainMenuTune;
	delete m_SettingsScreenObject;
	m_SettingsScreenObject = NULL;
}


PushdownState::PushdownResult ToonMainMenu::OnUpdate(float dt, PushdownState** newState)
{
	if (InputManager::GetInstance().GetInputs()[1]->IsPushingDown())
	{
		optionClick->ResetSound();
		AudioSystem::GetAudioSystem()->AddSoundEmitter(optionClick);
		m_MouseLastPosition = InputManager::GetInstance().GetInputs()[1]->GetMousePosition();
		UpdateMosePointerState(false);
		m_CurrentSelectedIndex = (m_CurrentSelectedIndex + 1) % m_mainMenuData.size();
	}
	if (InputManager::GetInstance().GetInputs()[1]->IsPushingUp())
	{
		optionClick->ResetSound();
		AudioSystem::GetAudioSystem()->AddSoundEmitter(optionClick);
		m_MouseLastPosition = InputManager::GetInstance().GetInputs()[1]->GetMousePosition();
		UpdateMosePointerState(false);
		m_CurrentSelectedIndex -= 1;
		if (m_CurrentSelectedIndex < 0) { m_CurrentSelectedIndex = (int)(m_mainMenuData.size()) - 1; }
	}

	if (!m_IsMousePointerVisible) { WakeMouseOnMovement(); }

	if (m_ShouldQuitGame) { return PushdownState::PushdownResult::Pop; }
	if (InputManager::GetInstance().GetInputs()[1]->IsBack()) 	//Keeping it to quit game on escape key press
	{
		if (!m_BaseCurrentSelectdIndex)
		{
			m_CurrentSelectedIndex = CONFIRMATION;
			return NavigateToScreen(newState);
		}
		else { return PushdownResult::Pop; }
	}

	if (InputManager::GetInstance().GetInputs()[1]->IsSelecting() || InputManager::GetInstance().GetInputs()[1]->IsShootingOnce() || m_HasUserInitiatedScreenNavigation)
	{
		return NavigateToScreen(newState);
	}
	m_Renderer->SetWorld(m_World);
	m_Renderer->Update(dt);
	m_Renderer->Render();
	Debug::UpdateRenderables(dt);
	DrawMainMenu();
	return PushdownResult::NoChange;
}

void ToonMainMenu::ToonMainMenuAudioInitialise() {
	mainMenuTune = new AudioEmitter();
	mainMenuTune->SetLooping(true);
	mainMenuTune->SetPriority(SoundPriority::ALWAYS);
	mainMenuTune->SetMusic();
	//mainMenuTune->SetVolume(0.2f);
	mainMenuTune->SetRadius(10000.0f);
	mainMenuTune->SetSound(Audio::GetSound("menuTune.wav"));
	AudioSystem::GetAudioSystem()->AddSoundEmitter(mainMenuTune);
	

	gameTune = new AudioEmitter();
	gameTune->SetLooping(true);
	gameTune->SetPriority(SoundPriority::ALWAYS);
	gameTune->SetMusic();
	//gameTune->SetVolume(0.002f);
	gameTune->SetRadius(1000000.0f);
	gameTune->SetSound(Audio::GetSound("gameTune.wav"));
	AudioSystem::GetAudioSystem()->AddSoundEmitter(gameTune);
	gameTune->Pause();

	optionClick = new AudioEmitter();
	optionClick->SetPriority(SoundPriority::ALWAYS);
	optionClick->SetMusic();
	optionClick->SetVolume(1.0f);
	optionClick->SetRadius(1000000.0f);
	optionClick->SetLooping(false);
	optionClick->SetSound(Audio::GetSound("splash.wav"));
}

void ToonMainMenu::SetAudioToMenu() {
	mainMenuTune->ResetSound();
	gameTune->Pause();

	mainMenuTune->Play();
}

void ToonMainMenu::SetAudioToGame() {
	mainMenuTune->Pause();
	gameTune->ResetSound();

	gameTune->Play();
}

void ToonMainMenu::OnAwake()
{
	if(mainMenuTune)
		SetAudioToMenu();

	AudioSystem::GetAudioSystem()->SetMainMenuStatus(true);

	UpdateMosePointerState(true);
	Window::GetWindow()->LockMouseToWindow(true);
	if (m_Game)
	{
		delete m_Game;
		m_Game = NULL;
	}
	if (m_LocalMenuScreenObject)
	{
		delete m_LocalMenuScreenObject;
		m_LocalMenuScreenObject = NULL;
	}
	if (m_MultiMenuScreenObject)
	{
		delete m_MultiMenuScreenObject;
		m_MultiMenuScreenObject = NULL;
	}
}

void ToonMainMenu::OnSleep()
{
	
}

bool ToonMainMenu::IsInside(Vector2 mouseCoordinates, MenuCoordinates singleMenuCoordinates)
{
	float widthConstraint  = singleMenuCoordinates.position.x + singleMenuCoordinates.size.x;
	float heightConstraint = singleMenuCoordinates.position.y + singleMenuCoordinates.size.y;
	return (mouseCoordinates.x >= singleMenuCoordinates.position.x && mouseCoordinates.x <= widthConstraint && mouseCoordinates.y >= singleMenuCoordinates.position.y && mouseCoordinates.y <= heightConstraint);
}


PushdownState::PushdownResult ToonMainMenu::NavigateToScreen(PushdownState** newState)
{
	std::vector<int> ipAddressVector;
	int navigationScreenIndex = m_CurrentSelectedIndex + (!m_HasUserInitiatedScreenNavigation ? m_BaseCurrentSelectdIndex : 0);
	m_HasUserInitiatedScreenNavigation = false;
	switch (navigationScreenIndex)
	{
	case PLAY:
		optionClick->ResetSound();
		AudioSystem::GetAudioSystem()->AddSoundEmitter(optionClick);
    
		*newState = GetLocalMenuSceenObject();
		break;
	case MULTIPLAY:
		optionClick->ResetSound();
		AudioSystem::GetAudioSystem()->AddSoundEmitter(optionClick);
    
		*newState = GetMultiMenuSceenObject();
		break;
	case SETTINGS:
		optionClick->ResetSound();
		AudioSystem::GetAudioSystem()->AddSoundEmitter(optionClick);
    
		*newState = GetSettingsScreenObject();
		break;
	case CREDITS:
		optionClick->ResetSound();
		AudioSystem::GetAudioSystem()->AddSoundEmitter(optionClick);

		return PushdownResult::NoChange;
	case QUIT:
		optionClick->ResetSound();
		AudioSystem::GetAudioSystem()->AddSoundEmitter(optionClick);

		*newState = m_ToonConfirmationScreen;
		break;
	case LAUNCH1PLAYER:
		m_Game = new ToonGame(m_Renderer);
		m_Game->m_WindowSize = m_Window->GetScreenSize();
		*newState = m_Game;
		break;
	case LAUNCH2PLAYER:
		m_Game = new ToonGame(m_Renderer, 2);
		m_Game->m_WindowSize = m_Window->GetScreenSize();
		*newState = m_Game;
		break;
	case LAUNCH3PLAYER:
		m_Game = new ToonGame(m_Renderer, 3);
		m_Game->m_WindowSize = m_Window->GetScreenSize();
		*newState = m_Game;
		break;
	case LAUNCH4PLAYER:
		m_Game = new ToonGame(m_Renderer, 4);
		m_Game->m_WindowSize = m_Window->GetScreenSize();
		*newState = m_Game;
		break;
	case BACKLOCAL:
		return PushdownResult::Pop;
	case LAUNCHASSERVER:
		SetAudioToGame();

		optionClick->ResetSound();
		AudioSystem::GetAudioSystem()->AddSoundEmitter(optionClick);

		m_Game	  = new ToonNetworkedGame(m_Renderer);
		m_Game->m_WindowSize = m_Window->GetScreenSize();
		*newState = m_Game;
		break;
	case LAUNCHASCLIENT:
		optionClick->ResetSound();
		AudioSystem::GetAudioSystem()->AddSoundEmitter(optionClick);
		*newState = GetUserInputScreenObject();
		break;
	case BACKMULTI:
		return PushdownResult::Pop;
	case PLAYAFTERSERIPSET:
		ipAddressVector = m_UserInputScreenObject->GetSeparatedIPAddressComponents();
		if (ipAddressVector.size() != 4) { return PushdownResult::NoChange; }
		m_Game							 = new ToonNetworkedGame(m_Renderer, ipAddressVector[0], ipAddressVector[1], ipAddressVector[2], ipAddressVector[3]);
		*newState						 = m_Game;
		AudioSystem::GetAudioSystem()->SetMainMenuStatus(false);
		break;
	case CONFIRMATION:
		*newState = m_ToonConfirmationScreen;
		break;
	}
	return PushdownResult::Push;
}

ToonMainMenu* ToonMainMenu::GetLocalMenuSceenObject()
{
	if (!m_LocalMenuScreenObject) { m_LocalMenuScreenObject = new ToonMainMenu(m_Renderer, m_LocalMainMenuData, GameStates::LAUNCH1PLAYER, m_World, m_Window, optionClick); }
	return m_LocalMenuScreenObject;
}

ToonMainMenu* ToonMainMenu::GetMultiMenuSceenObject()
{
	if (!m_MultiMenuScreenObject) { m_MultiMenuScreenObject = new ToonMainMenu(m_Renderer, m_MultiMainMenuData, GameStates::LAUNCHASSERVER, m_World, m_Window, optionClick); }
	return m_MultiMenuScreenObject;
}

ToonTextInput* ToonMainMenu::GetUserInputScreenObject()
{
	if (!m_UserInputScreenObject) { m_UserInputScreenObject = new ToonTextInput(Coordinates(Vector2(35, 20), Vector2(30, 5)), m_Renderer, m_Window->GetScreenSize(),
		[&](std::string inputStringText, bool* showError)
		{
			std::cout << inputStringText << std::endl;
			bool isValidIPAddress = std::regex_match(inputStringText, std::regex("(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"));
			if (isValidIPAddress)
			{
				m_HasUserInitiatedScreenNavigation = true;
				m_CurrentSelectedIndex			   = GameStates::PLAYAFTERSERIPSET;
				*showError						   = false;
				return PushdownResult::Pop;
			}
			else
			{
				*showError = true;
				return PushdownResult::NoChange;
			}
			return PushdownResult::NoChange;
		}, "Please enter a valid IP Address.");
	}
	return m_UserInputScreenObject;
}

void ToonMainMenu::UpdateMosePointerState(bool isVisible)
{
	Window::GetWindow()->ShowOSPointer(isVisible);
	m_IsMousePointerVisible = isVisible;
}

void ToonMainMenu::WakeMouseOnMovement()
{
	Vector2 currentMousePosition = InputManager::GetInstance().GetInputs()[1]->GetMousePosition();
	if (currentMousePosition != m_MouseLastPosition) { UpdateMosePointerState(true); }
	m_MouseLastPosition = currentMousePosition;
}

ToonGameSettings* ToonMainMenu::GetSettingsScreenObject()
{
	if (!m_SettingsScreenObject) { m_SettingsScreenObject = new ToonGameSettings(m_Renderer, m_World, m_Window); }
	return m_SettingsScreenObject;
}

PushdownState::PushdownResult ToonMainMenu::DidSelectCancelButton()
{
	return PushdownResult::Pop;
}

PushdownState::PushdownResult ToonMainMenu::DidSelectOkButton()
{
	m_ShouldQuitGame = true;
	return PushdownResult::Pop;
}

void ToonMainMenu::DrawMainMenu()
{
	if (!m_IsMousePointerVisible)
	{
		int index = 0;
		for (auto data : m_mainMenuData)
		{
			data.colour = m_CurrentSelectedIndex == index ? m_HoverColour : m_NormalTextColour;
			Debug::Print(data.text, data.coordinates.position, data.colour);
			index++;
		}
	}
	else
	{
		Vector2 mousePosition			  = InputManager::GetInstance().GetInputs()[1]->GetMousePosition();
		Vector2 windowSize				  = m_Window->GetWindow()->GetScreenSize();
		float	y						  = ((mousePosition.y / windowSize.y) * 100) + 5.0f;
		float	x						  = ((mousePosition.x / windowSize.x) * 100) + 5.0f;
		Vector2 mousePositionWithinBounds = Vector2(x, y);
		int		index					  = 0;
		for (auto data : m_mainMenuData)
		{
			data.colour			   = m_NormalTextColour;
			m_CurrentSelectedIndex = IsInside(mousePositionWithinBounds, data.coordinates) ? index : m_CurrentSelectedIndex;
			data.colour			   = index == m_CurrentSelectedIndex ? m_HoverColour : m_NormalTextColour;
			Debug::Print(data.text, data.coordinates.position, data.colour);
			index++;
		}
	}
}