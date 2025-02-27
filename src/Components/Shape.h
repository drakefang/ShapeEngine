
#pragma once

#include "raylib.h"
#include <array>

namespace ShapeGame
{
    template <int N> struct Shape
    {
        std::array<Vector2, N> vertices;
        Color color;
        float thickness;
        bool has_cap;
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

    struct TriangleComponent
    {
        Vector2 LeftBottom;
        Vector2 RightBottom;
    };
} // namespace ShapeGame