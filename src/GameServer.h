#pragma once

#include "GameHost.h"
#include "ServerPlayer.h"

#include <deque>
#include <map>

class GameServer : public GameHost
{
public:
	GameServer(enet_uint16 port);

	void broadcast(ENetPacket* packet);

	void update(uint32_t time);

	void onConnect(ENetPeer* sender);

	void onDisconnect(ENetPeer* sender);

	void onInput(ENetPeer* sender, uint32_t time, const Ship::Input& input, const std::deque<Ship::Move>& importantMoves);

	std::map<uint32_t, ServerPlayer> players;
private:
	void handleNetEvent(const ENetEvent& enetEvent);

	bool online;
	uint32_t hostId;
};
