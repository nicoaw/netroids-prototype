#include "GameEngine.h"

#include <enet/enet.h>

#include <GL/glew.h>
#include <GL/gl.h>

std::shared_ptr<GameState> GameEngine::currentState = nullptr;
float GameEngine::fixedTimestep;
SDL_GLContext GameEngine::renderContext;
bool GameEngine::running = false;
SDL_Window* GameEngine::window;

void GameEngine::changeState(std::shared_ptr<GameState> state)
{
	if (state != nullptr)
		currentState = state;
}

void GameEngine::cleanupENet()
{
	enet_deinitialize();
}

void GameEngine::closeDisplay()
{
	SDL_GL_DeleteContext(renderContext);
	SDL_DestroyWindow(window);
}

void GameEngine::handleSystemEvent(const SDL_Event& sdlEvent)
{
	switch (sdlEvent.type)
	{
	case SDL_KEYDOWN:
		currentState->onKeyPressed(sdlEvent.key.keysym.sym);
		break;
	case SDL_KEYUP:
		currentState->onKeyReleased(sdlEvent.key.keysym.sym);
		break;
	case SDL_QUIT:
		stop();
		break;
	}
}

void GameEngine::init()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
}

void GameEngine::initENet()
{
	enet_initialize();
}

void GameEngine::initOpenGL()
{
	glewInit();
	glGetError();

	glEnable(GL_MULTISAMPLE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

bool GameEngine::isRunning()
{
	return running;
}

bool GameEngine::openDisplay(const std::string& title, int width, int height)
{
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	if (window == nullptr)
		return false;

	renderContext = SDL_GL_CreateContext(window);

	SDL_GL_SetSwapInterval(1);

	return true;
}

void GameEngine::render(float alpha)
{
	if (currentState != nullptr)
		currentState->render(alpha);
}

void GameEngine::start(std::shared_ptr<GameState> state, float timestep)
{
	currentState = state;
	fixedTimestep = timestep;
	running = true;
}

void GameEngine::stop()
{
	running = false;
}

float GameEngine::timestep()
{
	return fixedTimestep;
}

void GameEngine::update(uint32_t time)
{
	if (currentState != nullptr)
		currentState->update(time);
}

void GameEngine::updateDisplay()
{
	SDL_GL_SwapWindow(window);

	if (currentState != nullptr)
	{
		SDL_Event sdlEvent;

		while (SDL_PollEvent(&sdlEvent))
		{
			handleSystemEvent(sdlEvent);
		}
	}
}
