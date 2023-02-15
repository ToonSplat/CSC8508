#include "GameServer.h"
#include "GameWorld.h"
#include "./enet/enet.h"
using namespace NCL;
using namespace CSC8503;

static std::map<int, ENetPeer*> playerMap;

GameServer::GameServer(int onPort, int maxClients) {
	port = onPort;
	clientMax = maxClients;
	clientCount = 0;
	netHandle = nullptr;
	Initialise();
}

GameServer::~GameServer() {
	Shutdown();
}

void GameServer::Shutdown() {
	SendGlobalPacket(BasicNetworkMessages::Shutdown);
	enet_host_destroy(netHandle);
	netHandle = nullptr;
}

bool GameServer::Initialise() {
	ENetAddress address;
	address.host = ENET_HOST_ANY;
	address.port = port;

	netHandle = enet_host_create(&address, clientMax, 1, 0, 0);

	if (!netHandle) {
		std::cout << __FUNCTION__ << " failed to create network handle!" << std::endl;
		return false;
	}
	return true;
}

bool GameServer::SendGlobalPacket(int msgID) {
	GamePacket packet;
	packet.type = msgID;
	return SendGlobalPacket(packet);
}

bool GameServer::SendGlobalPacket(GamePacket& packet, bool reliable) {
	ENetPacket* dataPacket = enet_packet_create(&packet, packet.GetTotalSize(), (reliable ? ENET_PACKET_FLAG_RELIABLE : 0));
	enet_host_broadcast(netHandle, 0, dataPacket);
	return true;
}

bool GameServer::SendPacketToClient(GamePacket& payload, int playerID, bool reliable) {
	ENetPeer* p = playerMap.find(playerID)->second;
	ENetPacket* dataPacket = enet_packet_create(&payload, payload.GetTotalSize(), (reliable ? ENET_PACKET_FLAG_RELIABLE : 0));
	enet_peer_send(p, 0, dataPacket);
	return true;
}

void GameServer::UpdateServer() {
	if (!netHandle)
		return;
	ENetEvent event;
	while (enet_host_service(netHandle, &event, 0) > 0) {
		int type = event.type;
		ENetPeer* p = event.peer;
		int peer = p->incomingPeerID;

		if (type == ENetEventType::ENET_EVENT_TYPE_CONNECT) {
			//std::cout << "Server: New client connected" << std::endl;
			clientCount++;
			playerMap.emplace(clientCount, p);
			ConnectPacket returnPacket(clientCount, true);
			SendPacketToClient(returnPacket, clientCount, true);
			//std::cout << "Sent to " << p << " that they are client " << clientCount << std::endl;
			ConnectPacket packet(clientCount, false);
			ProcessPacket(&packet);
		}
		else if (type == ENetEventType::ENET_EVENT_TYPE_DISCONNECT) {
			std::cout << "Server: A client has disconnected" << std::endl;
			for (auto i = playerMap.begin(); i != playerMap.end(); i++)
				if ((*i).second == p) {
					DisconnectPacket packet((*i).first);
					ProcessPacket(&packet);
					playerMap.erase(i);
					break;
				}
		}
		else if (type == ENetEventType::ENET_EVENT_TYPE_RECEIVE) {
			//std::cout << "Server: Packet received..." << std::endl;
			GamePacket* packet = (GamePacket*)event.packet->data;
			ProcessPacket(packet, peer);
			enet_packet_destroy(event.packet);
		}
	}
}

void GameServer::SetGameWorld(GameWorld& g) {
	gameWorld = &g;
}