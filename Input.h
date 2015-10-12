#pragma once

#include <SDL2/SDL_events.h>

class KeyListener
{
public:
	virtual void onKeyPressed(SDL_Keycode key) = 0;

	virtual void onKeyReleased(SDL_Keycode key) = 0;
};
