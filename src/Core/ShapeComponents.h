
#pragma once

#include "Core/BaseComponents.h"
#include "glm/ext/vector_float2.hpp"

namespace ShapeGame
{
    struct Thickness
    {
        float value = 1.f;
    };

    struct RoundedCap
    {
        uint8_t value:1 = true;
    };

    struct LineSegment : public BaseGeometry
    {
        glm::vec2 left;
        glm::vec2 right;
    };
}