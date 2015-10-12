#include "Bullet.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <vector>

GLuint Bullet::program;
GLuint Bullet::vertexArray;

Bullet::Bullet(glm::vec2 position, glm::vec2 velocity)
    : color(1.0f, 1.0f, 1.0f, 1.0f)
{
    current.position = position;
    current.velocity = std::move(velocity);
}

void Bullet::init(GLuint program)
{
    Bullet::program = program;

	std::vector<glm::vec2> positions =
	{
		glm::vec2(0.0f, 0.0f)
	};

	GLuint positionBuffer = graphics::createBuffer(GL_ARRAY_BUFFER, positions.begin(), positions.end(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);

	GLuint positionLocation = glGetAttribLocation(program, "position");

	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(positionLocation);
}

void Bullet::integrate(const AABB& bounds, State& state, float dt)
{
    state.position += dt * state.velocity;

    wrap(bounds, state);
}

Bullet::State Bullet::interpolate(const State& lhs, const State& rhs, float alpha)
{
    State state = rhs;
	state.position = lhs.position * (1.0f - alpha) + rhs.position * alpha;
	return state;
}

void Bullet::render(const Camera& camera, float alpha) const
{
    State state = interpolate(previous, current, alpha);

    glm::mat4 model(1.0f);

	model = glm::translate(model, glm::vec3(state.position, 0.0f));

	glUseProgram(program);
	glBindVertexArray(vertexArray);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));
	glUniform4fv(glGetUniformLocation(program, "color"), 1, glm::value_ptr(color));

	glDrawArrays(GL_POINTS, 0, 1);
}

void Bullet::snap(const State& state)
{
    current = previous = state;
}

const Bullet::State& Bullet::state() const
{
    return current;
}

void Bullet::update(const AABB& bounds, float dt)
{
    previous = current;
    integrate(bounds, current, dt);
}

void Bullet::wrap(const AABB& bounds, State& state)
{
    if (state.position.x < bounds.min.x)
		state.position.x = bounds.max.x + std::fmod(state.position.x, bounds.min.x);
	else if (state.position.x > bounds.max.x)
		state.position.x = bounds.min.x + std::fmod(state.position.x, bounds.max.x);

	if (state.position.y < bounds.min.y)
		state.position.y = bounds.max.y + std::fmod(state.position.y, bounds.min.y);
	else if (state.position.y > bounds.max.y)
		state.position.y = bounds.min.y + std::fmod(state.position.y, bounds.max.y);
}
