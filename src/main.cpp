
#include "Components/Shape.h"
#include "Components/Transform.h"
#include "entt/entt.hpp"
#include "raylib.h"

#include "Systems/RenderSystem.h"
#include <memory>


int main(int argc, char** argv)
{    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(1024, 768, "ShapeGame");
    SetTargetFPS(60);

    entt::registry registry;
    auto render = std::make_unique<ShapeGame::RenderSystem>(registry);

    auto line = registry.create();
    registry.emplace<ShapeGame::Segment>(line, 100.f, 10.f, true, RED);
    registry.emplace<ShapeGame::Location>(line, Vector2{ 0.f, -100.f}); 
    registry.emplace<ShapeGame::Rotation>(line, -30.f);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(SKYBLUE);

        render->Update(GetFrameTime());

        EndDrawing();
    }
    CloseWindow();
    return 0;
}