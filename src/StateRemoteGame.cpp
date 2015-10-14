#include "StateRemoteGame.h"

StateRemoteGame::StateRemoteGame(const std::string& hostName)
	: client(hostName, 4000), camera(800.0f, 600.0f)
{
	client.flush();

	client.ship.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

void StateRemoteGame::onKeyPressed(SDL_Keycode key)
{
	switch (key)
    {
    case SDLK_w:
        client.input.forward = true;
        break;
    case SDLK_a:
        client.input.left = true;
        break;
    case SDLK_d:
        client.input.right = true;
        break;
    }
}

void StateRemoteGame::onKeyReleased(SDL_Keycode key)
{
	switch (key)
    {
    case SDLK_w:
        client.input.forward = false;
        break;
    case SDLK_a:
        client.input.left = false;
        break;
    case SDLK_d:
        client.input.right = false;
        break;
    }
}

void StateRemoteGame::render(float alpha)
{
	glClear(GL_COLOR_BUFFER_BIT);

	client.ship.render(camera, alpha);

	for (auto& elem : client.players)
	{
		elem.second.ship.color = glm::vec4(0.0f, 1.0f, 0.0f, 0.5f);
		elem.second.ship.render(camera, alpha);
	}
}

void StateRemoteGame::update(uint32_t time)
{
	client.service();

	client.update(time);
}
