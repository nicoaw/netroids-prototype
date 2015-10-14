#pragma once

#include "GameState.h"
#include "GameClient.h"
#include "Camera.h"

class StateRemoteGame : public GameState
{
public:
	StateRemoteGame(const std::string& hostName);

	void onKeyPressed(SDL_Keycode key);

	void onKeyReleased(SDL_Keycode key);

	void update(uint32_t time);

	void render(float alpha);
private:
	GameClient client;
	Camera camera;
};