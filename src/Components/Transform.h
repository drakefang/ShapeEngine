

#pragma once


#include "raymath.h"

namespace ShapeGame
{
    // in world space
    struct Location 
    {
        Vector2 pos;
    };

    // in world space
    struct Direction
    {
        Vector2 dir;
    };

    // in world space
    struct Rotation
    {
        float rot;
    };
}