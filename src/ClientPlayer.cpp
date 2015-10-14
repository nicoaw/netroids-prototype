#include "ClientPlayer.h"

#include "GameEngine.h"

ClientPlayer::ClientPlayer()
	: Player()
{

}

void ClientPlayer::onSpawn(uint32_t time, const Ship::State& state)
{
	this->time = time;
	lastSyncTime = time;
	ship.snap(state);
}

void ClientPlayer::onSynchronize(uint32_t time, const Ship::State& state, const Ship::Input& input)
{
	if (time < lastSyncTime)
		return;

	lastSyncTime = time;
	updating = true;

	this->input = input;

	if (state.compare(ship.state()))
	{
		ship.snap(state);
	}
}