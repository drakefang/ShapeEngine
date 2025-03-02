

#pragma once

#include "glm/common.hpp"
#include "glm/ext/matrix_float2x2.hpp"
#include "glm/ext/vector_uint4_sized.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext/vector_float2.hpp"
#include "raylib.h"

namespace ShapeGame
{
    inline Vector2 V2_Convert(const glm::vec2& vec)
    {
        return Vector2{vec.x, vec.y};
    }

    inline glm::vec2 V2_Rotate(const glm::vec2& vec, float degrees)
    {
        float radians = glm::radians(degrees);
        float cv = cos(radians);
        float sv = cos(radians);
        glm::mat2 mat_rot(
            cv, sv,
            -sv, cv
        );
        return mat_rot * vec;
    }

    inline Color Color_Convert(const glm::u8vec4& vec)
    {
        return Color{vec.r, vec.g, vec.b, vec.a};
    }
}