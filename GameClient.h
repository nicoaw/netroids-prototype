#pragma once

#include "GameHost.h"
#include "MoveHistory.h"
#include "ClientPlayer.h"

#include <map>

class GameClient : public GameHost
{
public:
	GameClient(const std::string& hostName, enet_uint16 port);

	void send(ENetPacket* packet);

	void update(uint32_t time);

	void onConnect(uint32_t id);

	void onDisconnect();

	void onSpawn(uint32_t time, const Ship::State& state);

	void onSynchronize(uint32_t time, const Ship::State& state, const Ship::Input& input);

	Ship ship;
	Ship::Input input;
	std::map<uint32_t, ClientPlayer> players;
	bool replaying;
private:
	void handleNetEvent(const ENetEvent& enetEvent);

	bool connected;
	uint32_t id;
	MoveHistory history;
};