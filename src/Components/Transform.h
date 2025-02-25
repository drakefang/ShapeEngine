

#pragma once


#include "raylib.h"

namespace ShapeGame
{
    // in world space
    struct LocationComponent 
    {
        Vector2 pos;
    };

    // in world space
    struct DirectionComponent
    {
        Vector2 dir;
    };

    // in world space
    struct RotationComponent
    {
        float rot;
    };

    struct VelocityComponent
    {
        Vector2 velocity;
    };

    struct AccelerateComponent
    {
        Vector2 accelerate;
    };

    struct RotSpeedComponent
    {
        float speed;
    };
}