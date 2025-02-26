#include "RenderSystem.h"
#include "Components/Shape.h"
#include "Components/Transform.h"
#include "Systems/System.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "raymath.h"
//#include <algorithm>
#include <vector>
#include <cmath>

namespace ShapeGame
{
    constexpr int CapSegmentCount = 16;
    float Deg2Rad(float deg)
    {
        return deg * PI / 180.f;
    }

    void RenderSystem::GetVertices(std::vector<Vector2>& vertices, const SegmentComponent& segment, 
        float thickness, const Vector2& loc, float rot, bool hasThickness, bool hasEndcap)
    {
        Vector2 local_start = Vector2{-segment.length * 0.5f, 0};
        local_start = Vector2Rotate(local_start, Deg2Rad(rot));
        Vector2 local_end = Vector2{segment.length * 0.5f, 0};
        local_end = Vector2Rotate(local_end, Deg2Rad(rot));

        Vector2 start = Vector2Add(loc, local_start);
        Vector2 end = Vector2Add(loc, local_end);

        if (!hasThickness)
        {
            vertices.push_back(start);
            vertices.push_back(end);
            return;
        }

        const float halfThick = thickness * 0.5f;
        const Vector2 dir = Vector2Normalize(Vector2Subtract(end, start));
        const Vector2 perpendicular = { -dir.y * halfThick, dir.x * halfThick };

        // ===== 线段主体（Triangle Strip） =====
        vertices.push_back(Vector2Subtract(start, perpendicular));  // 上起点
        vertices.push_back(Vector2Add(start, perpendicular)); // 下起点
        vertices.push_back(Vector2Subtract(end, perpendicular));    // 上终点
        vertices.push_back(Vector2Add(end, perpendicular));   // 下终点

        if (hasEndcap) 
        { // ===== 端点圆形（Triangle Fan） =====
            auto addCircle = [&](Vector2 center, float angleOffset) 
            {
                vertices.push_back(center); // 中心点
                for (int i = 0; i <= CapSegmentCount; ++i) 
                {
                    float angle = -(angleOffset + (i * PI) / CapSegmentCount) + PI * 0.5f;
                    Vector2 offset = {cosf(angle) * halfThick, sinf(angle) * halfThick};
                    vertices.push_back(Vector2Add(center, offset));
                }
            };

            // 起点圆形（偏移PI/2保证连接方向正确）
            addCircle(start, atan2f(-perpendicular.x, -perpendicular.y));
            // 终点圆形（保持与线段方向一致）
            addCircle(end, atan2f(perpendicular.x, perpendicular.y));
        }

        /*
        std::for_each(vertices.begin(), vertices.end(), [this](Vector2& vert)
        {
            vert = GetWorldToScreen2D(vert, camera);
        });
        */
    }

    RenderSystem::RenderSystem(entt::registry& reg) : System(reg)
    {
        camera.target = {0.f, 0.f};
        camera.offset = { GetRenderWidth() * 0.5f, GetRenderHeight() * 0.5f};
        camera.rotation = 180.f;
        camera.zoom = 1.f;
    }

    void RenderSystem::Update(float DeltaTime)
    {
        BeginMode2D(camera);

        DrawLine(-1000.f, 0, 1000.f, 0, BLUE);
        DrawLine(0.f, -1000.f, 0.f, 1000.f, GREEN);

        auto view = registry.view<SegmentComponent, LocationComponent, RotationComponent>();
        view.each([this](const auto& entity, SegmentComponent segment, LocationComponent loc, RotationComponent rot)
        {
            std::vector<Vector2> vertices;
            bool hasThickness = registry.any_of<ThicknessComponent>(entity);
            bool has_endcap = registry.any_of<EndCapComponent>(entity);
            const auto &tc = registry.get<ThicknessComponent>(entity);
            Color c = BEIGE;
            bool hasColor = registry.any_of<ColorComponent>(entity);
            if (hasColor) 
            {
              c = registry.get<ColorComponent>(entity).color;
            }
            GetVertices(vertices, segment, tc.thickness, loc.pos, rot.rot, true,
                        has_endcap);
            if (hasThickness) 
            {
              // 绘制线段主体（Triangle Strip）
              DrawTriangleStrip(vertices.data(), 4, c);
              if (has_endcap) 
              {
                // 绘制两个端点圆形（Triangle Fan）
                DrawTriangleFan(vertices.data() + 4, CapSegmentCount + 2,
                                c);
                DrawTriangleFan(vertices.data() + 4 + CapSegmentCount + 2,
                                CapSegmentCount + 2, c);
              }
            }
            else 
            {
                DrawLineStrip(vertices.data(), 2, c);
            }
        });

        EndMode2D();
    }
}