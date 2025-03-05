
#include "PolygonRenderer.h"
#include "Core/BaseComponents.h"
#include "Core/GameContext.h"
#include "Core/ShapeComponents.h"
#include "Core/MathUtil.h"
#include "raylib.h"
#include <algorithm>
#include <cstring>
#include <ranges>

namespace ShapeGame
{
    static size_t GetVertices(
        std::vector<Vector2>& vertices,
        const Polygon& poly,
        float thickness,
        const glm::vec2& loc,
        float rot,
        bool filled,
        bool hasThickness,
        int segCount)
    {
        std::vector<glm::vec2> tmp = poly.vertices;
        std::for_each(
            tmp.begin(),
            tmp.end(),
            [loc, rot](auto& v)
            {
                v = V2_Rotate(v, rot);
                v += loc;
            });

        if (filled) // use triangle fan
        {
            tmp.push_back(*tmp.begin());
            tmp.emplace(tmp.begin(), loc);
            V2_Copy(vertices, tmp);
            return vertices.size();
        }
        if (hasThickness)
        {
            size_t res = 0;
            for (auto it = tmp.begin(); it != tmp.end(); ++it)
            {
                auto next = std::next(it);
                if (next == tmp.end())
                {
                    next = tmp.begin();
                }
                res = GetSegmentVerticesWithThickness(vertices, *it, *next, thickness, segCount);
            }
            return res;
        }
        else
        {
            tmp.push_back(*tmp.begin());
            V2_Copy(vertices, tmp);
            return vertices.size();
        }
    }

    void PolygonRenderer::Execute()
    {
        auto& registry = GameContext::Get().Registry();
        auto view = registry.view<Transform, Polygon, Visible>();
        view.each(
            [this](const auto& entity, const Transform& transform, const Polygon& poly, const Visible& visible)
            {
                if (poly.vertices.size() < 3)
                {
                    return;
                }
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
                int segCount = 0;
                if (hasEndCap)
                {
                    segCount = registry.get<RoundedCap>(entity).segments;
                }
                bool hasFilled = registry.any_of<Filled>(entity);
                bool filled = false;
                if (hasFilled)
                {
                    filled = registry.get<Filled>(entity).value;
                }
                std::vector<Vector2> vertices;
                size_t count = GetVertices(
                    vertices,
                    poly,
                    thickness,
                    transform.position,
                    transform.rotation,
                    filled,
                    hasThickness,
                    segCount);
                Color c = Color_Convert(poly.color);
                if (filled)
                {
                    DrawTriangleFan(vertices.data(), vertices.size(), c);
                }
                else
                {
                    if (hasThickness)
                    {
                        size_t lineCount = vertices.size() / count;
                        for (int i = 0; i < lineCount; i++)
                        {
                            const Vector2* points = vertices.data() + i * count;
                            DrawTriangleStrip(points, 4, c);
                            if (segCount > 0)
                            {
                                // 绘制两个端点圆形（Triangle Fan）
                                DrawTriangleFan(points + 4, segCount + 2, c);
                                DrawTriangleFan(points + 4 + segCount + 2, segCount + 2, c);
                            }
                        }
                    }
                    else
                    {
                        DrawLineStrip(vertices.data(), vertices.size(), c);
                    }
                }
            });
    }
} // namespace ShapeGame