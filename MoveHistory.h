#pragma once

#include "Ship.h"

#include <deque>
#include <cstdint>

class GameClient;

class MoveHistory
{
public:
	void push(const Ship::Move& move);

	void correction(GameClient& game, uint32_t time, const Ship::State& state, const Ship::Input& input);

	void getImportantMoves(std::deque<Ship::Move>& importantMoves);
private:
	std::deque<Ship::Move> moves, importantMoves;
};