#pragma once

#include <glm\glm.hpp>

struct Camera
{
public:
	Camera(float width, float height);

	glm::mat4 projection, view;
};