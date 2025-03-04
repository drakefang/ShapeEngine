
#include "RenderSystem.h"
#include "Core/BaseComponents.h"
#include "Core/GameContext.h"
#include "Core/Renderer/LineRenderer.h"
#include "Core/ShapeComponents.h"
#include <memory>

namespace ShapeGame
{
    RenderSystem::RenderSystem()
    {
        camera.target = {0.f, 0.f};
        camera.offset = {GetRenderWidth() * 0.5f, GetRenderHeight() * 0.5f};
        camera.rotation = 180.f;
        camera.zoom = 1.f;

        strategies.emplace(typeid(LineSegment), std::make_unique<LineRenderer>());
    }

    void RenderSystem::Update(float DeltaTime)
    {
        // 注意坐标轴，x轴向右，y轴向下
        BeginMode2D(camera);

        DrawLine(-1000.f, 0, 1000.f, 0, BLUE);
        DrawLine(0.f, -1000.f, 0.f, 1000.f, GREEN);

        auto& registry = GameContext::Get().Registry();
        registry.sort<BaseGeometry>(
            [](const BaseGeometry& lhs, const BaseGeometry& rhs)
            {
                if (lhs.group < rhs.group)
                {
                    return true;
                }
                if (lhs.group == rhs.group)
                {
                    return lhs.depth < rhs.depth;
                }
                if (lhs.group > rhs.group)
                {
                    return false;
                }
                return false;
            });
        for (auto& [type, strategy] : strategies)
        {
            strategy->Execute();
        }

        EndMode2D();
    }
} // namespace ShapeGame