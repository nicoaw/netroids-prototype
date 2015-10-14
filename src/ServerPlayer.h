#pragma once

#include "Player.h"

#include <enet/enet.h>

#include <deque>

class ServerPlayer : public Player
{
public:
	ServerPlayer();
	ServerPlayer(ENetPeer* peer, uint32_t time);

	void send(ENetPacket* packet);

	void onInput(uint32_t time, const AABB& bounds, const Ship::Input& input, const std::deque<Ship::Move>& importantMoves);

	ENetPeer* peer;
};
