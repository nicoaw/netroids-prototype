#include "Player.h"

#include "GameEngine.h"

Player::Player()
{
	input.forward = false;
	input.left = false;
	input.right = false;
}

void Player::update(const AABB& bounds, uint32_t time)
{
	ship.update(input, bounds, GameEngine::timestep());

	++this->time;
}