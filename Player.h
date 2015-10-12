#pragma once

#include "Ship.h"

#include <cstdint>

class Player
{
public:
	Player();

	void update(const AABB& bounds, uint32_t time);

	uint32_t time;
	Ship ship;
	Ship::Input input;
};