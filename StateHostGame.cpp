#include "StateHostGame.h"

StateHostGame::StateHostGame()
    : server(4000), client("localhost", 4000), camera(800.0f, 600.0f)
{
    client.flush();

    client.ship.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

void StateHostGame::onKeyPressed(SDL_Keycode key)
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
    case SDLK_SPACE:
        client.input.shoot = true;
        break;
    }
}

void StateHostGame::onKeyReleased(SDL_Keycode key)
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
    case SDLK_SPACE:
        client.input.shoot = false;
        break;
    }
}

void StateHostGame::render(float alpha)
{
    glClear(GL_COLOR_BUFFER_BIT);

    client.ship.render(camera, alpha);

    for (auto& elem : client.players)
    {
        elem.second.ship.color = glm::vec4(0.0f, 1.0f, 0.0f, 0.5f);
        elem.second.ship.render(camera, alpha);
    }
}

void StateHostGame::update(uint32_t time)
{
    client.service();
    server.service();

    client.update(time);
}
