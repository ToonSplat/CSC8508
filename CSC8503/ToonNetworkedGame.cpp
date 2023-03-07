#include "ToonNetworkedGame.h"
#include "ToonNetworkPackets.h"
#include "ToonNetworkObject.h"
#include "GameServer.h"
#include "GameClient.h"
#include "Player.h"
#include "ToonFollowCamera.h"
#include "ToonMinimapCamera.h"
#include "InputManager.h"
#include "ToonDebugManager.h"
#include "Maths.h"

#include <fstream>

#define COLLISION_MSG 30

ToonNetworkedGame::ToonNetworkedGame(GameTechRenderer* renderer) : ToonGame(renderer, 1, false) {
	thisServer = nullptr;
	thisClient = nullptr;

	NetworkBase::Initialise();
	timeToNextPacket = 0.0f;
	packetsToSnapshot = 0;
	world->SetNetworkStatus(NetworkingStatus::Server);
	world->GetEventListener()->AddServer(this);
	StartAsServer();
}

ToonNetworkedGame::ToonNetworkedGame(GameTechRenderer* renderer, int a, int b, int c, int d) : ToonGame(renderer, 1, false) {
	thisServer = nullptr;
	thisClient = nullptr;

	e = new AudioEmitter(Audio::GetSound("splash.wav"));

	NetworkBase::Initialise();
	timeToNextPacket = 0.0f;
	packetsToSnapshot = 0;
	world->SetNetworkStatus(NetworkingStatus::Client);
	StartAsClient(a, b, c, d);
}

ToonNetworkedGame::~ToonNetworkedGame() {
	delete thisServer;
	delete thisClient;
	delete e;
}

void ToonNetworkedGame::StartAsServer() {
	thisServer = new GameServer(NetworkBase::GetDefaultPort(), 4);

	thisServer->RegisterPacketHandler(Received_State, this);
	thisServer->RegisterPacketHandler(Player_Connected, this);
	thisServer->RegisterPacketHandler(Player_Disconnected, this);
	thisServer->RegisterPacketHandler(Client_Update, this);
	ServerStartGame();
}

void ToonNetworkedGame::StartAsClient(char a, char b, char c, char d) {
	thisClient = new GameClient();
	if (!thisClient->Connect(a, b, c, d, NetworkBase::GetDefaultPort())) {
		closeGame = true;
		return;
	}

	thisClient->RegisterPacketHandler(Delta_State, this);
	thisClient->RegisterPacketHandler(Full_State, this);
	thisClient->RegisterPacketHandler(Player_Connected, this);
	thisClient->RegisterPacketHandler(Player_Disconnected, this);
	thisClient->RegisterPacketHandler(Shoot, this);
	thisClient->RegisterPacketHandler(Impact, this);
	thisClient->RegisterPacketHandler(Message, this);

	StartGame();
}

PushdownState::PushdownResult ToonNetworkedGame::OnUpdate(float dt, PushdownState** newState) {
	if (serverClosed != -256.0f) {
		serverClosed -= dt;
		if (serverClosed <= 0) {
			thisServer->Shutdown();
			ToonDebugManager::Instance().SetGameWorld(nullptr);
			return PushdownResult::Pop;
		}
	}

	if (m_ShouldShowConfirmationScreen)
	{
		Window::GetWindow()->ShowOSPointer(true);
		if (!m_ConfirmationScreen)
		{
			m_ToonConfirmationScreen = new ToonConfirmationScreen(Coordinates(Vector2(30, 20), Vector2(50, 20)), m_WindowSize, renderer, "Are you sure, you want to quit the game?");
			m_ToonConfirmationScreen->delegate = this;
			m_ToonConfirmationScreen->m_ShouldRenderUpdates = false;
		}
		*newState = m_ToonConfirmationScreen;
		return PushdownResult::Push;
		//m_ToonConfirmationScreen->OnUpdate(dt, NULL);
	}


	if (m_MoveBackOnConfirmation || closeGame)
	{
		if (thisServer && serverClosed == -256.0f) {
			std::cout << "Beginning server shutdown, will be closed in 3 seconds\n";
			thisServer->RemoveClients();
			// Give everyone 3 seconds to get cleanly kicked off server
			serverClosed = 3.0f;
			return PushdownResult::NoChange;
		}
		else if (thisClient) {
			if(thisClient->IsConnected())
				thisClient->DisconnectFromServer();
			ToonDebugManager::Instance().SetGameWorld(nullptr);
			return PushdownResult::Pop;
		}
	}


	if (InputManager::GetInstance().GetInputs()[1]->IsBack()) {
		m_ShouldShowConfirmationScreen = true;
		//if (thisServer && serverClosed == -256.0f) {
		//	std::cout << "Beginning server shutdown, will be closed in 3 seconds\n";
		//	thisServer->RemoveClients();
		//	// Give everyone 3 seconds to get cleanly kicked off server
		//	serverClosed = 3.0f;
		//	return PushdownResult::NoChange;
		//}
		//else if (thisClient && thisClient->IsConnected()) {
		//	thisClient->DisconnectFromServer();
		//	return PushdownResult::Pop;
		//}
	}
	return ToonGame::OnUpdate(dt, newState);
}

void ToonNetworkedGame::UpdateGame(float dt) {
	;
	ToonDebugManager::Instance().StartNetworking();
	if (thisServer)
		Debug::Print("Server", Vector2(0, 5));
	else
		Debug::Print("Player ID: " + std::to_string(myID), Vector2(0, 5));
	if (gameTime <= 0 && thisServer) {
		if (winner == nullptr) {
			winner = DetermineWinner(renderer->GetTeamScores());
			MessagePacket endGame(3);
			endGame.messageValue = winner->GetTeamID();
			thisServer->SendGlobalPacket(endGame, true);
		}
		if (gameTime <= -5.0f) {
			ServerStartGame();
			return;
		}
	}
	timeToNextPacket -= dt;
	if (timeToNextPacket < 0) {
		if (thisServer) {
			UpdateAsServer(dt);
		}
		else if (thisClient) {
			UpdateAsClient(dt);
		}
		timeToNextPacket += 1.0f / 60.0f; //60hz server/client update
	}

	if (thisServer) {
		for (auto& player : serverPlayers) {
			PlayerControl* playersControl = player.second.controls;
			player.second.player->MovementUpdate(dt, playersControl);
			if (player.second.player->WeaponUpdate(dt, playersControl)) {
				reactphysics3d::Vector3 orientation = player.second.player->GetRigidbody()->getTransform().getOrientation() * reactphysics3d::Quaternion::fromEulerAngles(reactphysics3d::Vector3(reactphysics3d::decimal((player.second.controls->camera[0] + 10) / 180.0f * Maths::PI), 0, 0)) * reactphysics3d::Vector3(0, 0, -10.0f); // TODO: Update this to Sunit's new method of getting angle
				reactphysics3d::Vector3 dirOri = orientation;
				dirOri.y = 0;
				dirOri.normalize();
				orientation.normalize();
				reactphysics3d::Vector3 position = player.second.player->GetRigidbody()->getTransform().getPosition() + dirOri * reactphysics3d::decimal(3) + reactphysics3d::Vector3(0, reactphysics3d::decimal(player.second.player->GetScale().y * 1.5), 0);
				player.second.player->GetWeapon().FireBullet(position, orientation);
				ShootPacket newPacket;
				newPacket.playerID = player.first;
				newPacket.position[0] = (int)(position.x * 1000);
				newPacket.position[1] = (int)(position.y * 1000);
				newPacket.position[2] = (int)(position.z * 1000);

				newPacket.orientation[0] = (short)(orientation.x * 1000);
				newPacket.orientation[1] = (short)(orientation.y * 1000);
				newPacket.orientation[2] = (short)(orientation.z * 1000);
				//std::cout << "I sent a shot. Hopefully it arrives!\n";
				thisServer->SendGlobalPacket(newPacket, true);
			}
		}
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8) && thisServer) {
		ServerStartGame();
		return;
	}
	ToonDebugManager::Instance().EndNetworking();
	if (!closeGame)
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
	if (!players[1]) return;

	ClientPacket newPacket;
	newPacket.playerID = myID;
	newPacket.lastID = myState;
	newPacket.controls = *playerControls[1];
	//if (newPacket.controls.shooting) std::cout << "I sent that I am shooting. Hopefully it arrives!\n";
	thisClient->SendPacket(newPacket, true);
	playerControls[1]->jumping = false;
	playerControls[1]->shooting = false;
}

void ToonNetworkedGame::BroadcastSnapshot(bool deltaFrame) {
	int minID = INT_MAX;

	for (auto& i : serverPlayers) {
		minID = min(minID, i.second.StateID);
	}

	MessagePacket timePacket(4);
	timePacket.messageValue = (int)(gameTime * 10);
	thisServer->SendGlobalPacket(timePacket);

	for (auto& object : networkObjects) {
		GamePacket* newPacket = nullptr;
		if (object->WritePacket(&newPacket, deltaFrame, minID)) {
			thisServer->SendGlobalPacket(*newPacket);
			delete newPacket;
		}
	}
}

void ToonNetworkedGame::UpdateMinimumState() {
	int minID = INT_MAX;

	for (auto& i : serverPlayers) {
		minID = min(minID, i.second.StateID);
	}

	if (minID == INT_MAX)
		minID = 0;

	//std::cout << "The minimum state achived is " << minID << std::endl;

	//every client has acknowledged reaching at least state minID
	//so we can get rid of any old states!

	for (ToonNetworkObject* i : networkObjects)
		i->UpdateStateHistory(minID);
}

Player* ToonNetworkedGame::SpawnPlayer(int playerID, Team* team) {
	Player* newPlayerCharacter = levelManager->AddPlayerToWorld(Vector3(20, 5, 0), team);
	newPlayerCharacter->SetWorldID(-playerID);
	ToonNetworkObject* netO = new ToonNetworkObject(newPlayerCharacter, -playerID, myState);
	newPlayerCharacter->SetWeapon(baseWeapon);
	serverPlayers.find(playerID)->second.player = newPlayerCharacter;
	networkObjects.push_back(netO);
	allPlayers.emplace(newPlayerCharacter);
	return newPlayerCharacter;
}

void ToonNetworkedGame::ServerStartGame() {
	myState = 0;
	// Reset the game world
	StartGame();
	// For clients too!
	MessagePacket resetPacket(1);
	thisServer->SendGlobalPacket(resetPacket, true);
	// Add all players in
	for (auto i = serverPlayers.begin(); i != serverPlayers.end(); i++) {
		Team* team = (*i).second.team;
		SpawnPlayer((*i).first, team);
		ConnectPacket outPacket((*i).first, false, team->GetTeamID());
		thisServer->SendGlobalPacket(outPacket, true);
	}
}

void ToonNetworkedGame::StartGame() {
	networkObjects.clear();
	allPlayers.clear();
	winner = nullptr;
	gameTime = 90.0f;
	levelManager->ResetLevel(&networkObjects);
}

void ToonNetworkedGame::ReceivePacket(int type, GamePacket* payload, int source) {
	if (type == Player_Connected) {
		ConnectPacket* realPacket = (ConnectPacket*)payload;
		int receivedID = realPacket->playerID;
		// This is the server getting a new player
		if (thisServer && realPacket->teamID == -1) {
			std::cout << "Server: New player connected\n";
			Team* team = world->GetTeamLeastPlayers();

			// Give the player their ID
			ConnectPacket returnPacket(receivedID, true);
			thisServer->SendPacketToClient(returnPacket, receivedID, true);

			// Tell every player to spawn the new player
			ConnectPacket newPlayerPacket(receivedID, false, team->GetTeamID());
			thisServer->SendGlobalPacket(newPlayerPacket, true);

			// Tell the new player to spawn the other players
			for (auto i = serverPlayers.begin(); i != serverPlayers.end(); i++) {
				ConnectPacket existingPlayerPacket((*i).first, false, (*i).second.team->GetTeamID());
				thisServer->SendPacketToClient(existingPlayerPacket, receivedID, true);
			}

			// Tell the new player of every existing paint splat
			for (ToonGameObject* po : world->GetPaintableObjects()) {
				if (dynamic_cast<PaintableObject*>(po)) {
					PaintableObject* object = (PaintableObject*)po;
					for (ImpactPoint& i : *object->GetImpactPoints())
						SendImpactPoint(i, object, receivedID);
				}
				else {
					Player* object = (Player*)po;
					for (ImpactPoint& i : *object->GetImpactPoints())
						SendImpactPoint(i, object, receivedID);
				}
			}


			// Have the server spawn the new player and add them to the networking lists
			serverPlayers.emplace(receivedID, PlayerDetails(nullptr, new PlayerControl(), team));
			Player* newPlayer = SpawnPlayer(receivedID, team);
			return;
		}

		// I am recieving my ID
		int teamID = realPacket->teamID;
		Team* team = world->GetTeams()[teamID];
		if (realPacket->you) {
			myID = receivedID;
			std::cout << "Recieved my ID, I am" << myID << std::endl;
			return;
		}

		std::cout << "Recieved message Player Connected, Spawning their player, they are player ID" << receivedID << " and team " << teamID << std::endl;
		serverPlayers.emplace(receivedID, PlayerDetails(nullptr, nullptr, team));
		Player* newPlayer = SpawnPlayer(receivedID, team);
		if (myID == receivedID) {
			players[1] = newPlayer;
			playerControls[1] = new PlayerControl();
			world->SetMainCamera(1, new ToonFollowCamera(world, players[1]));
			world->SetMinimapCamera(new ToonMinimapCamera(*players[1]));
		}
	}
	else if (type == Player_Disconnected) {
		DisconnectPacket* realPacket = (DisconnectPacket*)payload;
		int receivedID = realPacket->playerID;
		// I am being told to disconnect, presumably because the server is closed
		if (receivedID == myID) {
			std::cout << "Server kicking me off\n";
			closeGame = true;
			return;
		}
		std::cout << "Recieved message Player Disconnected, removing their player, they are player ID" << receivedID << std::endl;
		Player* removingPlayer = serverPlayers.find(receivedID)->second.player;
		allPlayers.erase(removingPlayer);
		for (auto i = networkObjects.begin(); i != networkObjects.end(); i++) {
			if (removingPlayer->GetNetworkObject() == (*i)) {
				networkObjects.erase(i);
				break;
			}
		}
		world->RemoveGameObject(removingPlayer, false);
		world->RemovePaintableObject(removingPlayer);
		if (thisServer) {
			delete serverPlayers.find(receivedID)->second.controls;
			DisconnectPacket outPacket(receivedID);
			thisServer->SendGlobalPacket(outPacket, true);
		}
		serverPlayers.erase(receivedID);
	}
	else if (type == Client_Update) {
		ClientPacket* realPacket = (ClientPacket*)payload;
		int receivedID = realPacket->playerID;
		//std::cout << "Recieved message Client Update, adjusting their controls, they are player ID" << receivedID << " and in State " << realPacket->lastID << std::endl;
		if (receivedID <= 0) {
			//std::cout << "The client doesnt seem to have their ID/goat yet, ignoring\n";
			return;
		}
		serverPlayers.find(receivedID)->second.StateID = realPacket->lastID;
		PlayerControl* playersControls = serverPlayers.find(receivedID)->second.controls;
		playersControls->direction[0] = realPacket->controls.direction[0];
		playersControls->direction[1] = realPacket->controls.direction[1];
		playersControls->camera[0] = realPacket->controls.camera[0];
		playersControls->camera[1] = realPacket->controls.camera[1];
		if (playersControls->aiming != realPacket->controls.aiming) {
			MessagePacket aimPacket(2);
			aimPacket.playerID = realPacket->playerID;
			aimPacket.messageValue = (realPacket->controls.aiming ? 1 : 0);
			thisServer->SendGlobalPacket(aimPacket);
		}
		playersControls->aiming = realPacket->controls.aiming;
		playersControls->jumping = realPacket->controls.jumping;
		playersControls->shooting = realPacket->controls.shooting;
	}
	else if (type == Full_State) {
		FullPacket* realPacket = (FullPacket*)payload;
		//std::cout << "Recieved FullPacket for object " << realPacket->objectID << " at object state " << realPacket->fullState.stateID << std::endl;
		myState = max(myState, realPacket->fullState.stateID);
		for (auto i : networkObjects)
			if (i->GetNetworkID() == realPacket->objectID) {
				i->ReadPacket(*realPacket);
				break;
			}
	}
	else if (type == Delta_State) {
		DeltaPacket* realPacket = (DeltaPacket*)payload;
		//std::cout << "Recieved DeltaPacket for object " << realPacket->objectID << " at object state " << realPacket->fullID << std::endl;
		for (auto i : networkObjects)
			if (i->GetNetworkID() == realPacket->objectID) {
				i->ReadPacket(*realPacket);
				break;
			}
	}
	else if (type == Shoot) {
		ShootPacket* realPacket = (ShootPacket*)payload;
		//std::cout << "Recieved ShootPacket for player " << realPacket->playerID << std::endl;
		Player* shootingPlayer = serverPlayers.find(realPacket->playerID)->second.player;
		shootingPlayer->GetWeapon().FireBullet(reactphysics3d::Vector3(realPacket->position[0] / 1000.0f, realPacket->position[1] / 1000.0f, realPacket->position[2] / 1000.0f),
			reactphysics3d::Vector3(realPacket->orientation[0] / 1000.0f, realPacket->orientation[1] / 1000.0f, realPacket->orientation[2] / 1000.0f));
	}
	else if (type == Impact) 
		HandleImpactPacket((ImpactPacket*)payload);
	else if (type == Message)
		HandleMessagePacket((MessagePacket*)payload);

	else std::cout << "Recieved unknown packet\n";
}

void ToonNetworkedGame::HandleImpactPacket(ImpactPacket* packet) {
	//std::cout << "Recieved ImpactPacket for object " << realPacket->objectID << std::endl;

	Team* team = world->GetTeams()[packet->teamID];
	Vector3 realPosition(packet->position[0] / 1000.0f, packet->position[1] / 1000.0f, packet->position[2] / 1000.0f);
	float impactRadius = (float)(packet->radius) / 10.0f;

	for (ToonGameObject* p : world->GetPaintableObjects())
		if (p->GetWorldID() == packet->objectID) {
			e->SetPriority(SoundPriority::LOW);
			e->SetLooping(false);
			e->ResetSound();
			e->SetTarget(realPosition);
			AudioSystem::GetAudioSystem()->AddSoundEmitter(e);

			if (dynamic_cast<PaintableObject*>(p)) {
				PaintableObject* object = (PaintableObject*)p;
				object->AddImpactPoint(ImpactPoint(realPosition, team, impactRadius));
			}
			else {
				Player* object = (Player*)p;
				object->AddImpactPoint(ImpactPoint(realPosition, team, impactRadius));
			}
			world->MapNeedsChecking(true);
			break;
		}
}

void ToonNetworkedGame::HandleMessagePacket(MessagePacket* packet) {
	//std::cout << "Recieved MessagePacket" << std::endl;

	switch (packet->messageID) {
	case(1):
		myState = 0;
		StartGame();
		break;
	case(2):
		for (auto& player : allPlayers) 
			if (player->GetNetworkObject()->GetNetworkID() == -packet->playerID) {
				player->SetAiming((packet->messageValue == 0 ? false : true));
				break;
			}
		break;
	case(3):
		if (packet->messageValue == 0)
			winner = tieTeam;
		else winner = world->GetTeams().find(packet->messageValue)->second;
		break;
	case(4):
		gameTime = packet->messageValue / 10.0f;
		break;
	default:
		std::cout << "Recieved unknown message\n";
	}
}

void ToonNetworkedGame::SendImpactPoint(ImpactPoint point, ToonGameObject* object, int playerID){
	ImpactPacket newPacket;
	newPacket.objectID = object->GetWorldID();
	newPacket.teamID = point.GetTeamID();
	newPacket.position[0] = (int)(point.GetImpactLocation().x * 1000.0f);
	newPacket.position[1] = (int)(point.GetImpactLocation().y * 1000.0f);
	newPacket.position[2] = (int)(point.GetImpactLocation().z * 1000.0f);
	newPacket.radius = (char)(point.GetImpactRadius() * 10.0f);
	if (playerID == -1)
		thisServer->SendGlobalPacket(newPacket, true);
	else {
		thisServer->SendPacketToClient(newPacket, playerID, true);
	}
}

void ToonNetworkedGame::UpdateCall(float dt)
{
	UpdateGame(dt);
}

PushdownState::PushdownResult ToonNetworkedGame::DidSelectCancelButton()
{
	m_ShouldShowConfirmationScreen = false;
	return PushdownResult::Pop;
}

PushdownState::PushdownResult ToonNetworkedGame::DidSelectOkButton()
{
	m_ShouldShowConfirmationScreen = false;
	m_MoveBackOnConfirmation = true;
	return PushdownResult::Pop;
}