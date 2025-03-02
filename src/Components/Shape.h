
#pragma once

#include "glm/ext/vector_float2.hpp"
#include "raylib.h"
#include "glm/vec4.hpp"
#include "glm/vec2.hpp"
#include <vector>

namespace ShapeGame
{
    struct BaseGeometryComponent
    {
        glm::vec4 color{1.0f};
    };

    struct LineSegmentComponent : public BaseGeometryComponent
    {
        glm::vec2 start;
        glm::vec2 end;
    };

    struct QuadComponent : public BaseGeometryComponent
    {
        glm::vec2 size{1.f};
    };

    struct CircleComponent : public BaseGeometryComponent
    {
        float radius = 1.f;
        uint32_t segments = 32;
    };

    struct PolygonComponent : public BaseGeometryComponent
    {
        std::vector<glm::vec2> vertices;
    };

    struct ThicknessComponent
    {
        float thickness;
    };

    struct ColorComponent
    {
        Color color;
    };

    struct EndCapComponent
    {
    };

    struct SegmentComponent
    {
        float length = 100.f;
    };
} // namespace ShapeGame