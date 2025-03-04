
#include "LineRenderer.h"
#include "Core/BaseComponents.h"
#include "Core/GameContext.h"
#include "Core/MathUtil.h"
#include "Core/ShapeComponents.h"
#include "glm/ext/vector_float2.hpp"
#include "glm/geometric.hpp"
#include "raylib.h"
#include "raymath.h"

namespace ShapeGame
{
    constexpr int CapSegmentCount = 16;
    static void GetVertices(
        std::vector<Vector2>& vertices,
        const LineSegment& segment,
        float thickness,
        const glm::vec2& loc,
        float rot,
        bool hasThickness,
        bool hasEndcap)
    {
        const glm::vec2 local_start = V2_Rotate(segment.left, rot);
        const glm::vec2 local_end = V2_Rotate(segment.right, rot);
        const glm::vec2 start = loc + local_start;
        const glm::vec2 end = loc + local_end;

        if (!hasThickness)
        {
            vertices.push_back(V2_Convert(start));
            vertices.push_back(V2_Convert(end));
            return;
        }

        const float halfThick = thickness * 0.5f;
        const glm::vec2 dir = glm::normalize(end - start);
        const glm::vec2 perpendicular = {-dir.y * halfThick, dir.x * halfThick};

        // ===== 线段主体（Triangle Strip） =====
        vertices.push_back(V2_Convert(start - perpendicular)); // 上起点
        vertices.push_back(V2_Convert(start + perpendicular)); // 下起点
        vertices.push_back(V2_Convert(end - perpendicular));   // 上终点
        vertices.push_back(V2_Convert(end + perpendicular));   // 下终点

        if (hasEndcap)
        { // ===== 端点圆形（Triangle Fan） =====
            auto addCircle = [&](const glm::vec2& center, float angleOffset)
            {
                vertices.push_back(V2_Convert(center)); // 中心点
                for (int i = 0; i <= CapSegmentCount; ++i)
                {
                    float angle = -(angleOffset + (i * PI) / CapSegmentCount) + PI * 0.5f;
                    glm::vec2 offset = {cosf(angle) * halfThick, sinf(angle) * halfThick};
                    vertices.push_back(V2_Convert(center + offset));
                }
            };

            // 起点圆形（偏移PI/2保证连接方向正确）
            addCircle(start, atan2f(-perpendicular.x, -perpendicular.y));
            // 终点圆形（保持与线段方向一致）
            addCircle(end, atan2f(perpendicular.x, perpendicular.y));
        }
    }

    void LineRenderer::Execute()
    {
        auto& registry = GameContext::Get().Registry();
        auto view = registry.view<Transform, LineSegment, Visible>();
        view.each(
            [this](const auto& entity, const Transform& trans, const LineSegment& line, const Visible& visible)
            {
                auto& registry = GameContext::Get().Registry();
                if (!visible.value)
                {
                    return;
                }
                bool hasThickness = registry.any_of<Thickness>(entity);
                float thickness = 1.f;
                if (hasThickness)
                {
                    thickness = registry.get<Thickness>(entity).value;
                }
                bool hasEndCap = registry.any_of<RoundedCap>(entity);
                bool isRounedCap = true;
                if (hasEndCap)
                {
                    isRounedCap = registry.get<RoundedCap>(entity).value;
                }
                std::vector<Vector2> vertices;
                GetVertices(vertices, line, thickness, trans.position, trans.rotation, hasThickness, isRounedCap);
                Color c = Color_Convert(line.color);
                if (hasThickness)
                {
                    // 绘制线段主体（Triangle Strip）
                    DrawTriangleStrip(vertices.data(), 4, c);
                    if (isRounedCap)
                    {
                        // 绘制两个端点圆形（Triangle Fan）
                        DrawTriangleFan(vertices.data() + 4, CapSegmentCount + 2, c);
                        DrawTriangleFan(vertices.data() + 4 + CapSegmentCount + 2, CapSegmentCount + 2, c);
                    }
                }
                else
                {
                    DrawLineStrip(vertices.data(), 2, c);
                }
            });
    }
} // namespace ShapeGame
