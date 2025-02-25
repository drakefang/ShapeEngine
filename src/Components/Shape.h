
#pragma once

#include "raylib.h"
#include <array>

namespace ShapeGame 
{
    template <int N> 
    struct Shape 
    {
        std::array<Vector2, N> vertices;
        Color color;
        float thickness;
        bool has_cap;
    };

    struct Segment
    {
        Vector2 offset = {0.f, 0.f};
        float length = 100.f;
        float thickness = 5.f;
        bool has_cap = true;
        Color color = BEIGE;
    };

    struct TriangleComp
    {

    };

    using Triangle = Shape<3>;
    using Rectangle = Shape<4>;
} // namespace ShapeGame