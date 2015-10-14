#pragma once

#include "Rendering.h"
#include "Camera.h"
#include "AABB.h"

#include <cstdint>

class Bullet
{
public:
    struct State
    {
        glm::vec2 position;

        glm::vec2 velocity;
    };

    Bullet(glm::vec2 position, glm::vec2 velocity);

    void update(const AABB& bounds, float dt);

    void render(const Camera& camera, float alpha) const;

    void snap(const State& state);

    const State& state() const;

    static void init(GLuint program);

    glm::vec4 color;
private:
    static State interpolate(const State& lhs, const State& rhs, float alpha);

    static void integrate(const AABB& bounds, State& state, float dt);

    static void wrap(const AABB& bounds, State& state);

    static GLuint program, vertexArray;

    State current, previous;
};
