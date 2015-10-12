#include "Ship.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

GLuint Ship::program;
GLuint Ship::vertexArray;

bool Ship::State::compare(const State& other) const
{
	const float threshold = 0.1f * 0.1f;

	return glm::pow(glm::length(other.position - position), 2.0f) > threshold || glm::abs(other.angle - angle) > threshold;
}

bool Ship::State::operator==(const State& other) const
{
	return position == other.position && angle == other.angle;
}

bool Ship::State::operator!=(const State& other) const
{
	return !(*this == other);
}

void Ship::State::recalculate()
{
	/*if (angle > 360.0f)
		angle = std::fmod(angle, 360.0f);
	else if (angle < 0.0f)
		angle = std::fmod(angle, -360.0f);*/
}

Ship::Ship()
	: color(1.0f, 1.0f, 1.0f, 1.0f)
{
	current.angle = 0.0f;
	current.angularVelocity = 0.0f;
	current.mass = 30.0f;
	current.scale = 15.0f;
	current.inertiaTensor = current.mass * current.scale * current.scale * (1.0f / 6.0f);
	current.inverseMass = 1.0f / current.mass;
	current.inverseInertiaTensor = 1.0f / current.inertiaTensor;
	current.position = glm::vec2(0.0f, 0.0f);
	current.velocity = glm::vec2(0.0f, 0.0f);
	current.recalculate();

	previous = current;
}

void Ship::control(const Input& input, const State& state, glm::vec2& force, float& torque)
{
	const float linear = 16000.0f;
	const float angular = 230000.0f;
	float a = glm::radians(state.angle + 90.0f);

	if (input.forward)
		force += linear * glm::vec2(glm::cos(a), glm::sin(a));

	if (input.left)
		torque += angular;

	if (input.right)
		torque -= angular;
}

void Ship::drag(const State& state, glm::vec2& force, float& torque)
{
	const float linear = 35.0f;
	const float angular = 1300.0f;

	force -= linear * state.velocity;
	torque -= angular * state.angularVelocity;
}

Ship::Derivative Ship::evaluate(const Input& input, State state, float dt, const Derivative& derivative)
{
	state.position += dt * derivative.velocity;
	state.angle += dt * derivative.angularVelocity;

	state.velocity += dt * derivative.acceleration;
	state.angularVelocity += dt * derivative.angularAcceleration;
	state.recalculate();

	Derivative output;
	output.velocity = state.velocity;
	output.angularVelocity = state.angularVelocity;

	glm::vec2 force;
	float torque;

	forces(input, state, force, torque);

	output.acceleration = state.inverseMass * force;
	output.angularAcceleration = state.inverseInertiaTensor * torque;

	return output;
}

void Ship::forces(const Input& input, const State& state, glm::vec2& force, float& torque)
{
	force = glm::vec2(0.0f, 0.0f);
	torque = 0.0f;

	drag(state, force, torque);
	control(input, state, force, torque);
}

void Ship::init(GLuint program)
{
	Ship::program = program;

	std::vector<glm::vec2> positions =
	{
		glm::vec2(0.0f, 0.5f),
		glm::vec2(-0.4f, -0.5f),
		glm::vec2(0.4f, -0.5f)
	};

	GLuint positionBuffer = graphics::createBuffer(GL_ARRAY_BUFFER, positions.begin(), positions.end(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);

	GLuint positionLocation = glGetAttribLocation(program, "position");

	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(positionLocation);
}

void Ship::integrate(const Input& input, const AABB& bounds, State& state, float dt)
{
	Derivative a = evaluate(input, state),
		b = evaluate(input, state, dt * 0.5f, a),
		c = evaluate(input, state, dt * 0.5f, b),
		d = evaluate(input, state, dt, c);

	const float k = 1.0f / 6.0f;

	glm::vec2 velocity = k * (a.velocity + 2.0f * (b.velocity + c.velocity) + d.velocity);
	float angularVelocity = k * (a.angularVelocity + 2.0f * (b.angularVelocity + c.angularVelocity) + d.angularVelocity);

	glm::vec2 acceleration = k * (a.acceleration + 2.0f * (b.acceleration + c.acceleration) + d.acceleration);
	float angularAcceleration = k * (a.angularAcceleration + 2.0f * (b.angularAcceleration + c.angularAcceleration) + d.angularAcceleration);

	state.position += dt * velocity;
	state.angle += dt * angularVelocity;

	state.velocity += dt * acceleration;
	state.angularVelocity += dt * angularAcceleration;
	state.recalculate();

	wrap(bounds, state);
}

Ship::State Ship::interpolate(const State& lhs, const State& rhs, float alpha)
{
	State state = rhs;

	state.position = lhs.position * (1.0f - alpha) + rhs.position * alpha;
	state.angle = lhs.angle, (rhs.angle - lhs.angle) * alpha;
	state.recalculate();

	return state;
}

void Ship::render(const Camera& camera, float alpha) const
{
	State state = interpolate(previous, current, alpha);

	glm::mat4 model(1.0f);

	model = glm::translate(model, glm::vec3(state.position, 0.0f));
	model = glm::rotate(model, state.angle, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(state.scale, state.scale, 1.0f));

	glUseProgram(program);
	glBindVertexArray(vertexArray);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));
	glUniform4fv(glGetUniformLocation(program, "color"), 1, glm::value_ptr(color));

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Ship::snap(const State& state)
{
	current = previous = state;
}

const Ship::State& Ship::state() const
{
	return current;
}

void Ship::update(const Input& input, const AABB& bounds, float dt)
{
	previous = current;
	integrate(input, bounds, current, dt);
}

void Ship::wrap(const AABB& bounds, State& state)
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
