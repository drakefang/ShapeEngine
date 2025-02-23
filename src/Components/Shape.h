
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
        float length;
        float thickness;
        bool has_cap;
        Color color;
    };

    using Triangle = Shape<3>;
    using Rectangle = Shape<4>;
} // namespace ShapeGame