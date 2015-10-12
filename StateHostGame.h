#pragma once

#include "GameState.h"
#include "GameServer.h"
#include "GameClient.h"
#include "Camera.h"

class StateHostGame : public GameState
{
public:
	StateHostGame();

	void onKeyPressed(SDL_Keycode key);

	void onKeyReleased(SDL_Keycode key);

	void update(uint32_t time);

	void render(float alpha);
private:
	GameServer server;
	GameClient client;
	Camera camera;
};