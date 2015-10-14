#pragma once

#include "Rendering.h"
#include "AABB.h"
#include "Camera.h"

#include <cstdint>

class Ship
{
public:
	struct Input
	{
		bool forward;
		bool left;
		bool right;
		bool shoot;
	};

	struct State
	{
		bool operator==(const State& other) const;

		bool operator!=(const State& other) const;

		bool compare(const State& other) const;

		void recalculate();

		glm::vec2 position;
		float angle;

		glm::vec2 velocity;
		float angularVelocity;

		float scale;
		float mass;
		float inverseMass;
		float inertiaTensor;
		float inverseInertiaTensor;
	};

	struct Move
	{
		uint32_t time;
		Input input;
		State state;
	};

	Ship();

	void update(const Input& input, const AABB& bounds, float dt);

	void render(const Camera& camera, float alpha) const;

	void snap(const State& state);

	const State& state() const;

	static void init(GLuint program);

	glm::vec4 color;
private:
	struct Derivative
	{
		glm::vec2 velocity;
		float angularVelocity;

		glm::vec2 acceleration;
		float angularAcceleration;
	};

	static Derivative evaluate(const Input& input, State state, float dt = 0.0f, const Derivative& derivative = Derivative{glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(0.0f, 0.0f)});

	static State interpolate(const State& lhs, const State& rhs, float alpha);

	static void integrate(const Input& input, const AABB& bounds, State& state, float dt);

	static void forces(const Input& input, const State& state, glm::vec2& force, float& torque);

	static void drag(const State& state, glm::vec2& force, float& torque);

	static void control(const Input& input, const State& state, glm::vec2& force, float& torque);

	static void wrap(const AABB& bounds, State& state);

    static GLuint program, vertexArray;

	State current, previous;
};
