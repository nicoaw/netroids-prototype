#pragma once

#include "Ship.h"
#include "AABB.h"

#include <enet\enet.h>

#include <cstdint>

class GameHost
{
public:
	GameHost();

	~GameHost();

	void service();

	void flush();

	void update(uint32_t time);

	uint32_t time;
	AABB bounds;
	/*Ship ship;
	Ship::Input input;*/
protected:
	virtual void handleNetEvent(const ENetEvent& enetEvent) = 0;

	ENetHost* host;
};
