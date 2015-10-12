#include "GameEngine.h"
#include "StateHostGame.h"
#include "StateRemoteGame.h"

#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>

void init();

int main(int argc, char* argv[])
{
	try
	{
		GameEngine::init();

		GameEngine::openDisplay("Multiplayer Testing", 800, 600);

		GameEngine::initOpenGL();

		GameEngine::initENet();

        init();

		auto absoluteTime = std::chrono::steady_clock::now();
		float accumulator = 0.0f;
		uint32_t time = 0;

		char input;
		std::cout << "Start (S)erver or Start (C)lient: ";
		std::cin >> input;

		switch (input)
		{
		case 's':
			GameEngine::start(std::make_shared<StateHostGame>(), 0.02f);
			break;
		case 'c':
		{
			std::string hostName;
			std::cout << "host: ";
			std::cin >> hostName;
			GameEngine::start(std::make_shared<StateRemoteGame>(hostName), 0.02f);
		}
			break;
		default:
			return 0;
		}

		while (GameEngine::isRunning())
		{
			auto newTime = std::chrono::steady_clock::now();
			float dt = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - absoluteTime).count() / 1000.0f;

			if (dt <= 0.0f)
				continue;

			absoluteTime = newTime;

			accumulator += dt;

			while (accumulator >= GameEngine::timestep())
			{
				GameEngine::update(time);
				accumulator -= GameEngine::timestep();
				++time;
			}

			GameEngine::render(accumulator / GameEngine::timestep());

			GameEngine::updateDisplay();
		}

		GameEngine::cleanupENet();

		GameEngine::closeDisplay();

	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		system("pause");
	}

	return 0;
}

void init()
{
    GLuint program;

    std::vector<GLuint> shaders;
	std::ifstream file("test.vert");

	shaders.push_back(graphics::createShader(GL_VERTEX_SHADER, std::string{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator < char >() }));

	file.close();
	file.open("test.frag");

	shaders.push_back(graphics::createShader(GL_FRAGMENT_SHADER, std::string{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator < char >() }));

	program = graphics::createProgram(shaders.begin(), shaders.end());

	Ship::init(program);
}
