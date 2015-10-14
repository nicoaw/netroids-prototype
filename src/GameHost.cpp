#include "GameHost.h"

GameHost::GameHost()
	: time(0)
{
	bounds.max = glm::vec2(400.0f, 300.0f);
	bounds.min = glm::vec2(-400.0f, -300.0f);

	/*input.forward = false;
	input.left = false;
	input.right = false;*/
}

GameHost::~GameHost()
{
	if (host != nullptr)
		enet_host_destroy(host);
}

void GameHost::flush()
{
	enet_host_flush(host);
}

void GameHost::service()
{
	ENetEvent enetEvent;

	while (enet_host_service(host, &enetEvent, 0))
		handleNetEvent(enetEvent);
}

void GameHost::update(uint32_t time)
{
	//ship.update(input, bounds, GameEngine::timestep());

	++this->time;
}

