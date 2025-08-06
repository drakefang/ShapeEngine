//
// Created by yuyzc on 2025/8/4.
//

#pragma once
#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

namespace ShapeGame
{
    struct Geometry
    {
        // 顶点坐标 (相对于实体原点)
        std::vector<glm::vec2> vertices;
        // 颜色 (RGBA, 0xAABBGGRR)
        uint32_t color = 0xFFFFFFFF; // 默认为白色
        float thickness = 2.0f;
    };

    struct RenderComponent
    {
        std::vector<Geometry> geometries;
    };
}