#include "ToonNetworkedGame.h"
#include "NetworkPlayer.h"
#include "NetworkObject.h"
#include "GameServer.h"
#include "GameClient.h"
#include "PhysicsObject.h"
#include "RenderObject.h"
#include "Player.h"
#include "ToonFollowCamera.h"
#include "ToonMinimapCamera.h"

#include <fstream>

#define COLLISION_MSG 30

ToonNetworkedGame::ToonNetworkedGame() : ToonGame(false) {
	thisServer = nullptr;
	thisClient = nullptr;

	NetworkBase::Initialise();
	timeToNextPacket = 0.0f;
	packetsToSnapshot = 0;
	StartAsServer();
}

ToonNetworkedGame::ToonNetworkedGame(int a, int b, int c, int d) : ToonGame(false) {
	thisServer = nullptr;
	thisClient = nullptr;

	NetworkBase::Initialise();
	timeToNextPacket = 0.0f;
	packetsToSnapshot = 0;
	StartAsClient(a, b, c, d);
}

ToonNetworkedGame::~ToonNetworkedGame() {
	delete thisServer;
	delete thisClient;
}

void ToonNetworkedGame::StartAsServer() {
	thisServer = new GameServer(NetworkBase::GetDefaultPort(), 4);

	thisServer->RegisterPacketHandler(Received_State, this);
	thisServer->RegisterPacketHandler(Player_Connected, this);
	thisServer->RegisterPacketHandler(Player_Disconnected, this);
	ServerStartLevel();
}

void ToonNetworkedGame::StartAsClient(char a, char b, char c, char d) {
	thisClient = new GameClient();
	if (!thisClient->Connect(a, b, c, d, NetworkBase::GetDefaultPort()))
		std::cout << "Could not connect!\n";

	thisClient->RegisterPacketHandler(Delta_State, this);
	thisClient->RegisterPacketHandler(Full_State, this);
	thisClient->RegisterPacketHandler(Player_Connected, this);
	thisClient->RegisterPacketHandler(Player_Disconnected, this);
	thisClient->RegisterPacketHandler(Message, this);

	StartLevel();
}

void ToonNetworkedGame::UpdateGame(float dt) {
	if(thisServer)
		Debug::Print("Server", Vector2(0, 5));
	else
		Debug::Print("Player ID: " + std::to_string(myID), Vector2(0, 5));
	timeToNextPacket -= dt;
	if (timeToNextPacket < 0) {
		if (thisServer) {
			UpdateAsServer(dt);
		}
		else if (thisClient) {
			UpdateAsClient(dt);
		}
		timeToNextPacket += 1.0f / 20.0f; //20hz server/client update
	}

	ToonGame::UpdateGame(dt);
}

void ToonNetworkedGame::UpdateAsServer(float dt) {
	thisServer->UpdateServer();
	myState++;
	packetsToSnapshot--;
	if (packetsToSnapshot < 0) {
		UpdateMinimumState();
		BroadcastSnapshot(false);
		packetsToSnapshot = 5;
	}
	else {
		BroadcastSnapshot(true);
	}
}

void ToonNetworkedGame::UpdateAsClient(float dt) {
	thisClient->UpdateClient();

	//ClientPacket newPacket;
	//newPacket.playerID = myID;
	//newPacket.lastID = myState;

	//if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
	//	//fire button pressed!
	//	newPacket.buttonstates[0] = 1;
	//}
	//else
	//	newPacket.buttonstates[0] = 0;
	//if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
	//	//fire button pressed!
	//	newPacket.buttonstates[1] = 1;
	//}
	//else
	//	newPacket.buttonstates[1] = 0;
	//if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
	//	//fire button pressed!
	//	newPacket.buttonstates[2] = 1;
	//}
	//else
	//	newPacket.buttonstates[2] = 0;
	//if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
	//	//fire button pressed!
	//	newPacket.buttonstates[3] = 1;
	//}
	//else
	//	newPacket.buttonstates[3] = 0;
	//newPacket.camera = (int)world->GetMainCamera()->GetYaw();
	//thisClient->SendPacket(newPacket);
}

void ToonNetworkedGame::BroadcastSnapshot(bool deltaFrame) {
	/*std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;

	world->GetObjectIterators(first, last);

	for (auto i = first; i != last; ++i) {
		NetworkObject* o = (*i)->GetNetworkObject();
		if (!o) {
			continue;
		}
		int minID = INT_MAX;

		for (auto i : stateIDs) {
			minID = min(minID, i.second);
		}

		if (minID == INT_MAX)
			minID = myState - 5;

		GamePacket* newPacket = nullptr;
		if (o->WritePacket(&newPacket, deltaFrame, minID)) {
			thisServer->SendGlobalPacket(*newPacket);
			delete newPacket;
		}
	}*/
}

void ToonNetworkedGame::UpdateMinimumState() {
	////Periodically remove old data from the server
	//int minID = INT_MAX;
	//int maxID = 0; //we could use this to see if a player is lagging behind?

	//for (auto i : stateIDs) {
	//	minID = min(minID, i.second);
	//	maxID = max(maxID, i.second);
	//}
	//if (minID = INT_MAX)
	//	minID = myState - 10;
	////every client has acknowledged reaching at least state minID
	////so we can get rid of any old states!
	//std::vector<GameObject*>::const_iterator first;
	//std::vector<GameObject*>::const_iterator last;
	//world->GetObjectIterators(first, last);

	//for (auto i = first; i != last; ++i) {
	//	NetworkObject* o = (*i)->GetNetworkObject();
	//	if (!o) {
	//		continue;
	//	}
	//	o->UpdateStateHistory(minID); //clear out old states so they arent taking up memory...
	//}
}

Player* ToonNetworkedGame::SpawnPlayer(int playerID) {
	Player* newPlayerCharacter = ToonLevelManager::Get()->AddPlayerToWorld(Vector3(-20, 5, -20), ToonGameWorld::Get()->GetTeamLeastPlayers());
	return newPlayerCharacter;
}

void ToonNetworkedGame::ServerStartLevel() {
	
}

void ToonNetworkedGame::StartLevel() {
	
}

void ToonNetworkedGame::ReceivePacket(int type, GamePacket* payload, int source) {
	if (type == Player_Connected) {
		ConnectPacket* realPacket = (ConnectPacket*)payload;
		// I am recieving my ID
		int receivedID = realPacket->playerID;
		if (realPacket->you) {
			myID = receivedID;
			//std::cout << "Recieved my ID, I am" << myID << std::endl;
			return;
		}


		//std::cout << "Recieved message Player Connected, Spawning their player, they are player ID" << receivedID << std::endl;
		Player* newPlayer = SpawnPlayer(receivedID);
		if (myID == receivedID) {
			player = newPlayer;
			player->SetWeapon(baseWeapon);
			world->SetMainCamera(new ToonFollowCamera(*player));
			world->SetMinimapCamera(new ToonMinimapCamera(*player));
		}
		if (thisServer) {
			ConnectPacket outPacket(receivedID, false);
			thisServer->SendGlobalPacket(outPacket);
			stateIDs.emplace(receivedID, 0);
			playerControls.emplace(receivedID, new PlayerControl());
			for (auto i = serverPlayers.begin(); i != serverPlayers.end(); i++) {
				if ((*i).first != receivedID) {
					ConnectPacket goatPacket((*i).first, false);
					thisServer->SendPacketToClient(goatPacket, receivedID);
				}
			}
		}
	}
	else if (type == Player_Disconnected) {
		DisconnectPacket* realPacket = (DisconnectPacket*)payload;
		int receivedID = realPacket->playerID;
		std::cout << "Recieved message Player Disconnected, removing their player, they are player ID" << receivedID << std::endl;
		Player* removingGoat = serverPlayers.find(receivedID)->second;
		world->RemoveGameObject(removingGoat, true);
		serverPlayers.erase(receivedID);
		if (thisServer) {
			delete playerControls.find(receivedID)->second;
			playerControls.erase(receivedID);
			stateIDs.erase(receivedID);
			DisconnectPacket outPacket(receivedID);
			thisServer->SendGlobalPacket(outPacket);
		}
	}
	else if (type == Client_Update) {
		ClientPacket* realPacket = (ClientPacket*)payload;
		int receivedID = realPacket->playerID;
		//std::cout << "Recieved message Client Update, adjusting their controls, they are player ID" << receivedID << " and in State " << realPacket->lastID << std::endl;
		if (receivedID < 10000) {
			//std::cout << "The client doesnt seem to have their ID/goat yet, ignoring\n";
			return;
		}
		stateIDs.find(receivedID)->second = realPacket->lastID;
		PlayerControl* playersControls = playerControls.find(receivedID)->second;
		if (playersControls->buttonstates[0] != realPacket->buttonstates[0]) {
			playersControls->buttonstates[0] = realPacket->buttonstates[0];
			//std::cout << "Client " << receivedID << ": CHANGE IN UP KEY\n";
		}
		if (playersControls->buttonstates[1] != realPacket->buttonstates[1]) {
			playersControls->buttonstates[1] = realPacket->buttonstates[1];
			//std::cout << "Client " << receivedID << ": CHANGE IN DOWN KEY\n";
		}
		if (playersControls->buttonstates[2] != realPacket->buttonstates[2]) {
			playersControls->buttonstates[2] = realPacket->buttonstates[2];
			//std::cout << "Client " << receivedID << ": CHANGE IN LEFT KEY\n";
		}
		if (playersControls->buttonstates[3] != realPacket->buttonstates[3]) {
			playersControls->buttonstates[3] = realPacket->buttonstates[3];
			//std::cout << "Client " << receivedID << ": CHANGE IN RIGHT KEY\n";
		}
		playersControls->camera[0] = realPacket->camera[0];
		playersControls->camera[1] = realPacket->camera[1];
	}
	else std::cout << "Recieved unknown packet\n";
}