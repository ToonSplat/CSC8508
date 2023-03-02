#include "Window.h"
#include "Win32Window.h"
#include "ToonAssetManager.h"
#include "Debug.h"

#include "StateMachine.h"
#include "StateTransition.h"
#include "State.h"

#include "GameServer.h"
#include "GameClient.h"

#include "ToonGame.h"
#include "ToonNetworkedGame.h"

#include "PushdownMachine.h"

#include "PushdownState.h"

#include "BehaviourNode.h"
#include "BehaviourSelector.h"
#include "BehaviourSequence.h"
#include "BehaviourAction.h"
#include "ToonMainMenu.h"

#include "KeyboardInput.h"
#include "XboxControllerInput.h"
#include "InputManager.h"

#include <Windows.h>
#include <Xinput.h>

#include "../ThirdParty/imgui/imgui.h"
#include "../ThirdParty/imgui/imgui_impl_opengl3.h"
#include "../ThirdParty/imgui/imgui_impl_win32.h"

using namespace NCL;
using namespace CSC8503;

#include <chrono>
#include <thread>
#include <sstream>

/*

The main function should look pretty familar to you!
We make a window, and then go into a while loop that repeatedly
runs our 'game' until we press escape. Instead of making a 'renderer'
and updating it, we instead make a whole game, and repeatedly update that,
instead. 

This time, we've added some extra functionality to the window class - we can
hide or show the 

*/

void StartPushdownAutomata(Window* w, ToonMainMenu* mainMenu) {
	PushdownMachine machine(mainMenu);
	while (w->UpdateWindow()) {
		float dt = w->GetTimer()->GetTimeDeltaSeconds();
		if (dt > 0.1f) {
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::PRIOR)) {
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NEXT)) {
			w->ShowConsole(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T)) {
			w->SetWindowPosition(0, 0);
		}

		w->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));
		InputManager::GetInstance().Update();
		if (!machine.Update(dt)) {
			return;
		}
	}
}

int main()
{
	Window* w = Window::CreateGameWindow("ToonSplat", 1280, 720);
	ToonAssetManager::Create();
	GameTechRenderer* renderer = new GameTechRenderer();

	// Controller settings
	XINPUT_STATE controllerState;
	DWORD result = XInputGetState(0, &controllerState);
	if (result == ERROR_SUCCESS)
	{
		std::cout << "Controller detected." << std::endl;
		InputManager::GetInstance().GetInputs().emplace(1, new XboxControllerInput(0));
	}
	else
	{
		std::cout << "No controller detected. Using keyboard input." << std::endl;
		InputManager::GetInstance().GetInputs().emplace(1, new KeyboardInput(Window::GetKeyboard(), Window::GetMouse()));
	}
	//Imgui 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(dynamic_cast<NCL::Win32Code::Win32Window*>(w)->GetHandle());
	ImGui_ImplOpenGL3_Init();

	if (!w->HasInitialised()) {
		return -1;
	}

	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);


	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!
	//TestBehaviourTree();

	ToonMainMenu* mainMenu = new ToonMainMenu(renderer, new ToonGameWorld(), w);
	StartPushdownAutomata(w, mainMenu);

	ToonAssetManager::Destroy();
	Window::DestroyGameWindow();
	//Imgui 
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}