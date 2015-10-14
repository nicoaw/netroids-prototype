#pragma once

#include "Input.h"

#include <cstdint>

class GameState : public KeyListener
{
public:
	virtual void update(uint32_t time) = 0;

	virtual void render(float alpha) = 0;
};