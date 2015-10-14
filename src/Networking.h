#pragma once

#include "Ship.h"
#include "Net.pb.h"

#include <enet/enet.h>

#include <vector>

namespace net
{
	const enet_uint8 RELIABLE_CHANNEL = 0;
	const enet_uint8 UNRELIABLE_CHANNEL = 1;

	ENetAddress makeAddress(const std::string& hostName, enet_uint16 port);

	ENetAddress makeAddress(enet_uint16 port);

	PacketBase parsePacketBase(ENetPacket* packet);

	template<typename T>
	T parsePacketData(const PacketBase& base)
	{
		T data;
		data.ParseFromArray(base.data().data(), base.data().size());

		return data;
	}

	ENetPacket* serializePacket(PacketType type, const google::protobuf::MessageLite& data, enet_uint32 flags);

	Ship::Input parseInput(const Input& input);

	Ship::State parseState(const State& state);

	Ship::Move parseMove(const Move& move);

	Input serializeInput(const Ship::Input& input);

	State serializeState(const Ship::State& state);

	Move serializeMove(const Ship::Move& move);
}
