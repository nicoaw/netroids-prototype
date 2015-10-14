#pragma once

#include "GameState.h"

#include <SDL2/SDL.h>

#include <string>
#include <memory>
#include <vector>

class GameEngine
{
public:
	static void init();

	static bool openDisplay(const std::string& title, int width, int height);

	static void closeDisplay();

	static void updateDisplay();

	static void initOpenGL();

	static void initENet();

	static void cleanupENet();

	static void start(std::shared_ptr<GameState> state, float timestep = 0.1f);

	static void stop();

	static void update(uint32_t time);

	static void render(float alpha);

	static void changeState(std::shared_ptr<GameState> state);

	static float timestep();

	static bool isRunning();
private:
	static void handleSystemEvent(const SDL_Event& sdlEvent);

	static float fixedTimestep;
	static bool running;
	static SDL_Window* window;
	static SDL_GLContext renderContext;
	static std::shared_ptr<GameState> currentState;
};
