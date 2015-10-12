#include "ServerPlayer.h"

#include "Networking.h"

ServerPlayer::ServerPlayer()
{

}

ServerPlayer::ServerPlayer(ENetPeer* peer, uint32_t time)
	: Player(), peer(peer)
{
	this->time = time;
}

void ServerPlayer::onInput(uint32_t time, const AABB& bounds, const Ship::Input& input, const std::deque<Ship::Move>& importantMoves)
{
	for (const Ship::Move& move : importantMoves)
	{
		while (this->time < move.time)
			update(bounds, this->time);

		this->input = move.input;
	}

	while (this->time < time)
		update(bounds, time);

	this->input = input;
}

void ServerPlayer::send(ENetPacket* packet)
{
	bool reliable = (packet->flags & ENET_PACKET_FLAG_RELIABLE) == ENET_PACKET_FLAG_RELIABLE;
	enet_peer_send(peer, (reliable ? net::RELIABLE_CHANNEL : net::UNRELIABLE_CHANNEL), packet);
}
