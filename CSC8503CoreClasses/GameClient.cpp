#include "GameClient.h"
#include "./enet/enet.h"
using namespace NCL;
using namespace CSC8503;

GameClient::GameClient() {
	netHandle = enet_host_create(nullptr, 1, 1, 0, 0);
}

GameClient::~GameClient() {
	if (netHandle) 
		enet_host_destroy(netHandle);
	netHandle = nullptr;
}

bool GameClient::Connect(uint8_t a, uint8_t b, uint8_t c, uint8_t d, int portNum) {
	ENetAddress address;
	ENetEvent event;
	address.port = portNum;
	address.host = (d << 24) | (c << 16) | (b << 8) | (a);

	netPeer = enet_host_connect(netHandle, &address, 2, 0);

	if (netPeer == NULL)
	{
		std::cout << "No available peers for initiating an ENet connection.\n";
		return false;
	}
	/* Wait up to 3 seconds for the connection attempt to succeed. */
	if (enet_host_service(netHandle, &event, 3000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT)
	{
		std::cout << "Connection to " << std::to_string(a) << "." << std::to_string(b) << "." << std::to_string(c) << "." << std::to_string(d) << ":" << portNum << " succeeded.";
		return true;
	}
	else
	{
		/* Either the 3 seconds are up or a disconnect event was */
		/* received. Reset the peer in the event the 3 seconds   */
		/* had run out without any significant event.            */
		enet_peer_reset(netPeer);
		delete netHandle;
		netHandle = nullptr;
		delete netPeer;
		netPeer = nullptr;
		std::cout << "Connection to " << std::to_string(a) << "." << std::to_string(b) << "." << std::to_string(c) << "." << std::to_string(d) << ":" << portNum << " failed.";
		return false;
	}
}

void GameClient::DisconnectFromServer() {
	if (netPeer) {
		ENetEvent event;
		enet_peer_disconnect(netPeer, 0);
		/* Allow up to 3 seconds for the disconnect to succeed
		 * and drop any packets received packets.
		 */
		while (enet_host_service(netHandle, &event, 3000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				puts("Disconnection succeeded.");
				return;
			}
		}
		/* We've arrived here, so the disconnect attempt didn't */
		/* succeed yet.  Force the connection down.             */
		puts("Forcefully closing connection.");
		enet_peer_reset(netPeer);
	}
}

void GameClient::UpdateClient() {
	if (netHandle == nullptr)
		return;
	// Handle all incoming packets
	ENetEvent event;
	while (enet_host_service(netHandle, &event, 0) > 0) {
		if (event.type == ENET_EVENT_TYPE_CONNECT)
			std::cout << "Connected to server!" << std::endl;
		else if (event.type = ENET_EVENT_TYPE_RECEIVE) {
			//std::cout << "Client: Packet received..." << std::endl;
			GamePacket* packet = (GamePacket*)event.packet->data;
			ProcessPacket(packet);
		}
		enet_packet_destroy(event.packet);
	}
}

void GameClient::SendPacket(GamePacket& payload, bool reliable) {
	ENetPacket* dataPacket = enet_packet_create(&payload, payload.GetTotalSize(), (reliable ? ENET_PACKET_FLAG_RELIABLE : 0));
	enet_peer_send(netPeer, 0, dataPacket);
	enet_host_flush(netHandle);
}
