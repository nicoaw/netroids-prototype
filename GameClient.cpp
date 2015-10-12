#include "GameClient.h"

#include "GameEngine.h"
#include "Networking.h"

GameClient::GameClient(const std::string& hostName, enet_uint16 port)
	: GameHost(), connected(false), replaying(false)
{
    host = enet_host_create(nullptr, 1, 2, 0, 0);

    ENetAddress address = net::makeAddress(hostName, port);
	enet_host_connect(host, &address, 2, 0);

	input.forward = false;
	input.left = false;
	input.right = false;
}

void GameClient::handleNetEvent(const ENetEvent& enetEvent)
{
	switch (enetEvent.type)
	{
	case ENET_EVENT_TYPE_CONNECT:
		if (!connected)
			onConnect(enetEvent.peer->connectID);
		break;
	case ENET_EVENT_TYPE_DISCONNECT:
		if (enetEvent.peer->connectID == id)
			onDisconnect();
		else
			players.erase(enetEvent.peer->connectID);
		break;
	case ENET_EVENT_TYPE_RECEIVE:
	{
		net::PacketBase base = net::parsePacketBase(enetEvent.packet);

		switch (base.type())
		{
		case net::PacketType::SPAWN:
		{
			auto data = net::parsePacketData<net::PacketSpawn>(base);

			if (data.id() == id)
				onSpawn(data.time(), net::parseState(data.state()));
			else
			{
				ClientPlayer& player = players[data.id()];
				players.at(data.id()).onSpawn(data.time(), net::parseState(data.state()));
			}
		}
			break;
		case net::PacketType::SYNC:
		{
			auto data = net::parsePacketData<net::PacketSync>(base);

			if (data.id() == id)
				onSynchronize(data.time(), net::parseState(data.state()), net::parseInput(data.input()));
			else
				players.at(data.id()).onSynchronize(data.time(), net::parseState(data.state()), net::parseInput(data.input()));
		}
			break;
		}
	}
		break;
	}
}

void GameClient::onConnect(uint32_t id)
{
	this->id = id;
	connected = true;
}

void GameClient::onDisconnect()
{
	connected = false;
}

void GameClient::onSpawn(uint32_t time, const Ship::State& state)
{
	this->time = time;
	ship.snap(state);
}

void GameClient::onSynchronize(uint32_t time, const Ship::State& state, const Ship::Input& input)
{
	history.correction(*this, time, state, input);
}

void GameClient::send(ENetPacket* packet)
{
	bool reliable = (packet->flags & ENET_PACKET_FLAG_RELIABLE) == ENET_PACKET_FLAG_RELIABLE;
	enet_peer_send(host->peers, (reliable ? net::RELIABLE_CHANNEL : net::UNRELIABLE_CHANNEL), packet);
}

void GameClient::update(uint32_t time)
{
	if (!replaying)
	{
		net::PacketInput data;
		net::Input* input = new net::Input;

		*input = net::serializeInput(this->input);

		data.set_allocated_input(input);
		data.set_time(this->time);

		std::deque<Ship::Move> importantMoves;
		history.getImportantMoves(importantMoves);

		for (const Ship::Move& move : importantMoves)
			*data.add_important_moves() = net::serializeMove(move);

		send(net::serializePacket(net::PacketType::INPUT, data, 0));

		history.push(Ship::Move{ time, this->input, ship.state() });

		for (auto& elem : players)
			elem.second.update(bounds, time);
	}

	ship.update(input, bounds, GameEngine::timestep());

	GameHost::update(time);
}
