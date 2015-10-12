#include "Camera.h"

#include <glm\gtc\matrix_transform.hpp>

Camera::Camera(float width, float height)
	: projection(glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f)), view(glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)))
{

}