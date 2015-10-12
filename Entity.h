#pragma once

#include "Rendering.h"
#include "AABB.h"

template<typename T, typename U>
class Entity
{
public:
    typedef T State;

    Entity();

    virtual void update(const AABB& bounds, float dt);

    virtual void render(const Camera& camera, float alpha);
protected:
    typedef U Derivative;

    State current, previous;
};
