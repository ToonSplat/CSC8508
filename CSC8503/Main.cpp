#include "Window.h"
#include "Win32Window.h"

#include "Debug.h"

#include "StateMachine.h"
#include "StateTransition.h"
#include "State.h"

#include "GameServer.h"
#include "GameClient.h"

#include "NavigationGrid.h"
#include "NavigationMesh.h"

#include "TutorialGame.h"
#include "ToonGame.h"
#include "ToonNetworkedGame.h"
#include "NetworkedGame.h"

#include "PushdownMachine.h"

#include "PushdownState.h"

#include "BehaviourNode.h"
#include "BehaviourSelector.h"
#include "BehaviourSequence.h"
#include "BehaviourAction.h"
#include "ToonMainMenu.h"

#include "../ThirdParty/imgui/imgui.h"
#include "../ThirdParty/imgui/imgui_impl_opengl3.h"
#include "../ThirdParty/imgui/imgui_impl_win32.h"

#include "AudioSystem.h"

using namespace NCL;
using namespace CSC8503;

#include <chrono>
#include <thread>
#include <sstream>

//Audio sounds
std::map<std::string, NCL::CSC8503::Sound*> NCL::CSC8503::Audio::soundEffectBuffers;

void AddAudioFiles() {
	Audio::AddSound("splatter.wav");
	Audio::AddSound("tune.wav");
}

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
		if (!machine.Update(dt)) {
			return;
		}
	}
}

int main()
{
	Window* w = Window::CreateGameWindow("ToonSplat", 1280, 720);
	GameTechRenderer* renderer = new GameTechRenderer();
	ToonGameWorld* world = new ToonGameWorld();

	//Imgui 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(dynamic_cast<NCL::Win32Code::Win32Window*>(w)->GetHandle());
	ImGui_ImplOpenGL3_Init();

	//OpenAL
	NCL::CSC8503::AudioSystem::Initialise();
	AddAudioFiles();

	if (!w->HasInitialised()) {
		return -1;
	}

	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);


	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!
	//TestBehaviourTree();

	ToonMainMenu* mainMenu = new ToonMainMenu(renderer, world, w);
	StartPushdownAutomata(w, mainMenu);

	Audio::DeleteSounds();
	AudioSystem::Destroy();

	Window::DestroyGameWindow();
	//Imgui 
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}