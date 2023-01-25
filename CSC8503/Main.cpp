#include "Window.h"

#include "Debug.h"

#include "StateMachine.h"
#include "StateTransition.h"
#include "State.h"

#include "GameServer.h"
#include "GameClient.h"

#include "NavigationGrid.h"
#include "NavigationMesh.h"

#include "TutorialGame.h"
#include "NetworkedGame.h"

#include "PushdownMachine.h"

#include "PushdownState.h"

#include "BehaviourNode.h"
#include "BehaviourSelector.h"
#include "BehaviourSequence.h"
#include "BehaviourAction.h"

using namespace NCL;
using namespace CSC8503;

#include <chrono>
#include <thread>
#include <sstream>

vector<Vector3> testNodes;
void TestPathFinding() {
	NavigationGrid grid("TestGrid1.txt");

	NavigationPath outPath;

	Vector3 startPos(80, 0, 10);
	Vector3 endPos(80, 0, 80);

	bool found = grid.FindPath(startPos, endPos, outPath);

	Vector3 pos;
	while (outPath.PopWaypoint(pos)) {
		testNodes.push_back(pos);
	}
}

void DisplayPathFinding() {
	for (int i = 1; i < testNodes.size(); ++i) {
		Vector3 a = testNodes[i - 1];
		Vector3 b = testNodes[i];

		Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
	}
}

void TestBehaviourTree() {
	float behaviourTimer;
	float distanceToTarget;
	BehaviourAction* findKey = new BehaviourAction("Find Key", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise) {
			std::cout << "Looking for a key" << std::endl;
			behaviourTimer = rand() % 100;
			state = Ongoing;
		}
		else if (state == Ongoing) {
			behaviourTimer -= dt;
			if (behaviourTimer <= 0.0f) {
				std::cout << "Found a key!" << std::endl;
				return Success;
			}
		}
		return state; // will return ongoing until timer runs out then return success
		}
	);

	BehaviourAction* goToRoom = new BehaviourAction("Go To Room", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise) {
			std::cout << "Going to room!" << std::endl;
			state = Ongoing;
		}
		else if (state == Ongoing) {
			distanceToTarget -= dt;
			if (distanceToTarget <= 0.0f) {
				std::cout << "Reached Room!" << std::endl;
				return Success;
			}
		}
		return state; // will return ongoing until reaches the room then return success
		}
	);

	BehaviourAction* openDoor = new BehaviourAction("Open Door", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise) {
			std::cout << "Opening Door!" << std::endl;
			return Success;
		}
		return state; // will return ongoing until reaches the room then return success
		}
	);

	BehaviourAction* lookForTreasure = new BehaviourAction("Look For Treasure", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise) {
			std::cout << "Looking for treasure!" << std::endl;
			state = Ongoing;
		}
		else if (state == Ongoing) {
			bool found = rand() % 2;
			if (found) {
				std::cout << "Found some treasure!" << std::endl;
				return Success;
			}
			std::cout << "No treasure here!" << std::endl;
			return Failure;
		}
		return state;
		}
	);

	BehaviourAction* lookForItems = new BehaviourAction("Look For Items", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise) {
			std::cout << "Looking for items!" << std::endl;
			state = Ongoing;
		}
		else if (state == Ongoing) {
			bool found = rand() % 2;
			if (found) {
				std::cout << "Found some items!" << std::endl;
				return Success;
			}
			std::cout << "No items here!" << std::endl;
			return Failure;
		}
		return state;
		}
	);

	BehaviourSequence* seq = new BehaviourSequence("Room Sequence");
	seq->AddChild(findKey);
	seq->AddChild(goToRoom);
	seq->AddChild(openDoor);

	BehaviourSelector* sel = new BehaviourSelector("Loot Selection");
	sel->AddChild(lookForTreasure);
	sel->AddChild(lookForItems);

	BehaviourSequence* rootSeq = new BehaviourSequence("Root Sequence");
	rootSeq->AddChild(seq);
	rootSeq->AddChild(sel);

	for (int i = 0; i < 5; ++i) {
		rootSeq->Reset();
		behaviourTimer = 0.0f;
		distanceToTarget = rand() % 250;
		BehaviourState state = Ongoing;
		std::cout << "Were going on a wounderful adventure!" << std::endl;
		while (state == Ongoing) {
			state = rootSeq->Execute(1.0f); // fake dt
		}
		if (state == Success) {
			std::cout << "That was a successful adventure!" << std::endl;
		}
		else if (state == Failure) {
			std::cout << "A complete waste of time!" << std::endl;
		}
	}
	std::cout << "All Done!" << std::endl;
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
int main() {
	Window*w = Window::CreateGameWindow("CSC8503 Game technology!", 1280, 720);

	if (!w->HasInitialised()) {
		return -1;
	}	

	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);

	TutorialGame* g = new TutorialGame();
	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!
	TestBehaviourTree();
	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		TestPathFinding();
		DisplayPathFinding();
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

		g->UpdateGame(dt);
	}
	Window::DestroyGameWindow();

}

void TestStateMachine() {
	StateMachine* testMachine = new StateMachine();
	int data = 0;

	//Create the different states
	State* A = new State([&](float dt)->void {
		std::cout << "Current State - A" << std::endl;
		data++;
		});

	State* B = new State([&](float dt)->void {
		std::cout << "Current State - B	" << std::endl;
		data--;
		});

	StateTransition* stateAB = new StateTransition(A, B, [&](void)->bool { return data > 10; }); // Go from state A to state B
	StateTransition* stateBA = new StateTransition(A, B, [&](void)->bool { return data < 0; }); // Go from state B to state A

	testMachine->AddState(A);
	testMachine->AddState(B);
	testMachine->AddTransition(stateAB);
	testMachine->AddTransition(stateBA);

	for (int i = 0; i < 100; ++i)
		testMachine->Update(1.0f);
}