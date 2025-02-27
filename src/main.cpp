
#include "Components/Shape.h"
#include "Components/Transform.h"
#include "entt/entt.hpp"
#include "raylib.h"

#include "Systems/RenderSystem.h"
#include "Systems/TransformSystem.h"
#include <memory>

int main(int argc, char** argv)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(1024, 768, "ShapeGame");
    SetTargetFPS(60);

    entt::registry registry;
    auto render = std::make_unique<ShapeGame::RenderSystem>(registry);
    auto rot = std::make_unique<ShapeGame::TransformSystem>(registry);

    auto line = registry.create();
    registry.emplace<ShapeGame::SegmentComponent>(line, 100.f);
    registry.emplace<ShapeGame::TransformComponent>(line, Vector2{0.f, 100.f}, -30.f, Vector2{1.f, 1.f});
    registry.emplace<ShapeGame::ThicknessComponent>(line, 10.f);
    registry.emplace<ShapeGame::EndCapComponent>(line);
    registry.emplace<ShapeGame::ColorComponent>(line, RED);
    registry.emplace<ShapeGame::RotatorComponent>(line, 360.f, 10.f);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(SKYBLUE);

        render->Update(GetFrameTime());
        rot->Update(GetFrameTime());

        EndDrawing();
    }
    CloseWindow();
    return 0;
}