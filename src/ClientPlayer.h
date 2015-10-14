#pragma once

#include "Player.h"
#include "Networking.h"

class ClientPlayer : public Player
{
public:
	ClientPlayer();

	void onSpawn(uint32_t time, const Ship::State& state);

	void onSynchronize(uint32_t time, const Ship::State& state, const Ship::Input& input);
private:
	uint32_t lastSyncTime;
	bool updating;
};