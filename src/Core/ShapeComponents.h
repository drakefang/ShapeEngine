
#pragma once

#include "Core/BaseComponents.h"
#include "glm/ext/vector_float2.hpp"
#include <cstdint>
#include <vector>

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

    struct Rect : BaseGeometry
    {
        glm::vec2 size;
        uint8_t filled:1 = false;
    };

    struct Polygon : BaseGeometry
    {
        std::vector<glm::vec2> vertices;
    };
}