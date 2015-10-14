#include "GameServer.h"

#include "Networking.h"
#include "GameEngine.h"

GameServer::GameServer(enet_uint16 port)
	: GameHost(), online(false)
{
    ENetAddress address = net::makeAddress(port);
    host = enet_host_create(&address, 32, 2, 0, 0);
}

void GameServer::broadcast(ENetPacket* packet)
{
	enet_host_broadcast(host, 0, packet);
}

void GameServer::handleNetEvent(const ENetEvent& enetEvent)
{
	switch (enetEvent.type)
	{
	case ENET_EVENT_TYPE_CONNECT:
		onConnect(enetEvent.peer);
		break;
	case ENET_EVENT_TYPE_DISCONNECT:
		onDisconnect(enetEvent.peer);
		break;
	case ENET_EVENT_TYPE_RECEIVE:
	{
		net::PacketBase base = net::parsePacketBase(enetEvent.packet);

		switch (base.type())
		{
		case net::PacketType::INPUT:
		{
			auto data = net::parsePacketData<net::PacketInput>(base);
			std::deque<Ship::Move> importantMoves;

			for (const net::Move& move : data.important_moves())
				importantMoves.push_back(net::parseMove(move));

			onInput(enetEvent.peer, (uint32_t) data.time(), net::parseInput(data.input()), importantMoves);
		}
			break;
		}
	}
		break;
	}
}

void GameServer::onConnect(ENetPeer* sender)
{
	if (!online)
	{
		hostId = sender->connectID;
		online = true;
	}

	ServerPlayer& player = players[sender->connectID];
	player = ServerPlayer(sender, time);

	net::PacketSpawn spawn;
	net::State* state = new net::State;

	*state = net::serializeState(player.ship.state());

	spawn.set_id(sender->connectID);
	spawn.set_time(time);
	spawn.set_allocated_state(state);

	broadcast(net::serializePacket(net::PacketType::SPAWN, spawn, ENET_PACKET_FLAG_RELIABLE));

	for (const auto& elem : players)
	{
		if (elem.first != player.peer->connectID)
		{
			net::State* peerState = new net::State;

			*peerState = net::serializeState(elem.second.ship.state());

			spawn.set_id(elem.first);
			spawn.set_allocated_state(peerState);

			player.send(net::serializePacket(net::PacketType::SPAWN, spawn, ENET_PACKET_FLAG_RELIABLE));
		}
	}
}

void GameServer::onDisconnect(ENetPeer* sender)
{
	players.erase(sender->connectID);

	if (sender->connectID == hostId)
	{
		online = false;

		players.clear();
		for (size_t i = 0; i < host->connectedPeers; ++i)
			enet_peer_disconnect(&host->peers[i], 0);

		flush();
	}
}

void GameServer::onInput(ENetPeer* sender, uint32_t time, const Ship::Input& input, const std::deque<Ship::Move>& importantMoves)
{
	ServerPlayer& player = players.at(sender->connectID);

	player.onInput(time, bounds, input, importantMoves);

	net::PacketSync data;
	net::Input* netInput = new net::Input;
	net::State* netState = new net::State;

	*netInput = net::serializeInput(player.input);
	*netState = net::serializeState(player.ship.state());

	data.set_id(sender->connectID);
	data.set_allocated_input(netInput);
	data.set_allocated_state(netState);
	data.set_time(player.time);

	broadcast(net::serializePacket(net::PacketType::SYNC, data, 0));
}
