
#include "LineRenderer.h"
#include "BaseComponents.h"
#include "GameContext.h"
#include "MathUtil.h"
#include "ShapeComponents.h"
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
        const glm::vec2& scale,
        float rot,
        bool hasThickness,
        int capSegCount)
    {
        const glm::vec2 local_start = V2_Rotate(segment.left, rot) * scale;
        const glm::vec2 local_end = V2_Rotate(segment.right, rot) * scale;
        const glm::vec2 start = loc + local_start;
        const glm::vec2 end = loc + local_end;

        if (!hasThickness)
        {
            vertices.push_back(V2_Convert(start));
            vertices.push_back(V2_Convert(end));
            return;
        }

        GetSegmentVerticesWithThickness(vertices, start, end, thickness, capSegCount);
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
                int capSegCount = 0;
                if (hasEndCap)
                {
                    capSegCount = registry.get<RoundedCap>(entity).segments;
                }
                std::vector<Vector2> vertices;
                GetVertices(vertices, line, thickness, trans.position, trans.scale, trans.rotation, hasThickness, capSegCount);
                Color c = Color_Convert(line.color);
                if (hasThickness)
                {
                    // 绘制线段主体（Triangle Strip）
                    DrawTriangleStrip(vertices.data(), 4, c);
                    if (capSegCount > 0)
                    {
                        // 绘制两个端点圆形（Triangle Fan）
                        DrawTriangleFan(vertices.data() + 4, capSegCount + 2, c);
                        DrawTriangleFan(vertices.data() + 4 + capSegCount + 2, capSegCount + 2, c);
                    }
                }
                else
                {
                    DrawLineStrip(vertices.data(), 2, c);
                }
            });
    }
} // namespace ShapeGame
