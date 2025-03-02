

#pragma once

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/ext/vector_uint4_sized.hpp"
#include "raylib.h"
#include <cstdint>

namespace ShapeGame
{
    struct Transform
    {
        glm::vec2 position {0, 0};
        float rotation = 0.f;
        glm::vec2 scale {1.f, 1.f};
    };

    enum class RenderGroup : uint8_t
    {
        BackGround = 0,
        Default = 100,
        UI = 200
    };

    struct BaseGeometry
    {
        glm::u8vec4 color{1};
        RenderGroup group = RenderGroup::Default;
        uint8_t depth = 0;
    };

    struct Visible
    {
        uint8_t value:1 = true;
    };
}