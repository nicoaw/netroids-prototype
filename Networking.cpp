#include "Networking.h"

#include <vector>
#include <cstdint>

namespace net
{
	ENetAddress makeAddress(const std::string& hostName, enet_uint16 port)
	{
		ENetAddress address;

		enet_address_set_host(&address, hostName.c_str());
		address.port = port;

		return address;
	}

	ENetAddress makeAddress(enet_uint16 port)
	{
		ENetAddress address;

		address.host = ENET_HOST_ANY;
		address.port = port;

		return address;
	}

	PacketBase parsePacketBase(ENetPacket* packet)
	{
		PacketBase base;
		base.ParseFromArray(packet->data, packet->dataLength);

		return base;
	}

	ENetPacket* serializePacket(PacketType type, const google::protobuf::MessageLite& data, enet_uint32 flags)
	{
		PacketBase base;
		std::vector<uint8_t> v;

		v.resize(data.ByteSize());
		data.SerializeToArray(v.data(), v.size());

		base.set_type(type);
		base.set_data(v.data(), v.size());

		v.resize(base.ByteSize());
		base.SerializeToArray(v.data(), v.size());

		return enet_packet_create(v.data(), v.size(), flags);
	}

	Ship::Input parseInput(const Input& input)
	{
		Ship::Input result;

		result.forward = input.forward();
		result.left = input.left();
		result.right = input.right();

		return result;
	}

	Ship::State parseState(const State& state)
	{
		Ship::State result;

		result.angle = state.angle();
		result.angularVelocity = state.angular_velocity();
		result.inertiaTensor = state.inertia_tensor();
		result.inverseInertiaTensor = 1.0f / result.inertiaTensor;
		result.mass = state.mass();
		result.inverseMass = 1.0f / result.mass;
		result.position = glm::vec2(state.position_x(), state.position_y());
		result.scale = state.scale();
		result.velocity = glm::vec2(state.velocity_x(), state.velocity_y());

		return result;
	}

	Ship::Move parseMove(const Move& move)
	{
		Ship::Move result;

		result.input = parseInput(move.input());
		result.state = parseState(move.state());
		result.time = move.time();

		return result;
	}

	Input serializeInput(const Ship::Input& input)
	{
		Input result;

		result.set_forward(input.forward);
		result.set_left(input.left);
		result.set_right(input.right);

		return result;
	}

	State serializeState(const Ship::State& state)
	{
		State result;

		result.set_angle(state.angle);
		result.set_angular_velocity(state.angularVelocity);
		result.set_inertia_tensor(state.inertiaTensor);
		result.set_mass(state.mass);
		result.set_position_x(state.position.x);
		result.set_position_y(state.position.y);
		result.set_scale(state.scale);
		result.set_velocity_x(state.velocity.x);
		result.set_velocity_y(state.velocity.y);

		return result;
	}

	Move serializeMove(const Ship::Move& move)
	{
		Move result;
		Input* input = new Input;
		State* state = new State;

		*input = serializeInput(move.input);
		*state = serializeState(move.state);

		result.set_allocated_input(input);
		result.set_allocated_state(state);
		result.set_time(move.time);

		return result;
	}
}
