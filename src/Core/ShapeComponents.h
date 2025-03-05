
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
        int segments = 16;
    };

    struct Filled
    {
        uint8_t value:1 = false;
    };

    struct LineSegment : public BaseGeometry
    {
        glm::vec2 left;
        glm::vec2 right;
    };

    struct Quad : public BaseGeometry
    {
        glm::vec2 size;
    };

    struct Polygon : public BaseGeometry
    {
        std::vector<glm::vec2> vertices;
    };

    struct Circle : public BaseGeometry
    {
        float radius = 10.f;
        int segments = 32;
    };
}