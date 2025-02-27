

#pragma once

#include "raylib.h"

namespace ShapeGame
{
    struct TransformComponent
    {
        Vector2 pos;
        float rot;
        Vector2 scale;
    };

    struct VelocityComponent
    {
        Vector2 velocity;
        Vector2 accelerate;
    };

    struct RotatorComponent
    {
        float target;
        float speed;
    };
} // namespace ShapeGame