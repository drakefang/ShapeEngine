
#include "PolygonRenderer.h"
#include "Core/BaseComponents.h"
#include "Core/GameContext.h"
#include "Core/ShapeComponents.h"
#include "Core/MathUtil.h"
#include "raylib.h"
#include <algorithm>
#include <cstring>

namespace ShapeGame
{
    static void GetVertices(
        std::vector<Vector2>& vertices,
        const Polygon& poly,
        float thickness,
        const glm::vec2& loc,
        float rot,
        bool filled,
        bool hasThickness,
        bool hasEndcap)
    {
        std::vector<glm::vec2> tmp = poly.vertices;
        tmp.emplace(tmp.begin(), loc);
        std::for_each(tmp.begin() + 1, tmp.end(), [loc, rot](auto& v)
        {
            v = V2_Rotate(v, rot);
            v += loc;
        });
        if (filled)
        {
            vertices.resize(tmp.size());
            std::memcpy(vertices.data(), tmp.data(), tmp.size() * sizeof(glm::vec2));
            return;
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
                bool isRounedCap = true;
                if (hasEndCap)
                {
                    isRounedCap = registry.get<RoundedCap>(entity).value;
                }
                bool hasFilled = registry.any_of<Filled>(entity);
                bool filled = false;
                if (hasFilled)
                {
                    filled = registry.get<Filled>(entity).value;
                }
                std::vector<Vector2> vertices;
                GetVertices(vertices, poly, thickness, transform.position, 
                    transform.rotation, filled, hasThickness, isRounedCap);
                Color c = Color_Convert(poly.color);
                if (filled)
                {
                    DrawTriangleFan(vertices.data(), vertices.size(), c);
                }
                else
                {
                    if (hasThickness)
                    {
                    }
                    else
                    {
                    }
                }
            });
    }
} // namespace ShapeGame