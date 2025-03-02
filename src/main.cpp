
#include "Components/Shape.h"
#include "Components/Transform.h"
#include "Core/BaseComponents.h"
#include "Core/GameContext.h"
#include "Core/ShapeComponents.h"
#include "Systems/System.h"
#include "entt/entt.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_uint4_sized.hpp"
#include "raylib.h"

#include "Core/Renderer/RenderSystem.h"
// #include "Systems/RenderSystem.h"
#include "Systems/TransformSystem.h"

using namespace ShapeGame;

int main(int argc, char** argv)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(1024, 768, "ShapeGame");
    SetTargetFPS(60);

    entt::registry registry;
    auto line = registry.create();
    registry.emplace<ShapeGame::SegmentComponent>(line, 100.f);
    registry.emplace<ShapeGame::TransformComponent>(line, Vector2{0.f, 100.f}, 30.f, Vector2{1.f, 1.f});
    registry.emplace<ShapeGame::ThicknessComponent>(line, 10.f);
    registry.emplace<ShapeGame::EndCapComponent>(line);
    registry.emplace<ShapeGame::ColorComponent>(line, RED);
    registry.emplace<ShapeGame::RotatorComponent>(line, 360.f, 30.f);

    GameContext::Get().RegisterSystem<RenderSystem>();
    auto seg = GameContext::Get().CreateEntity();
    LineSegment& sc = GameContext::Get().AddComponent<LineSegment>(seg);
    sc.left = glm::vec2{-100.f, 0};
    sc.right = glm::vec2{100.f, 0};
    sc.color = glm::u8vec4{255, 0, 0, 255};
    ShapeGame::Transform& tf = GameContext::Get().AddComponent<ShapeGame::Transform>(seg);
    tf.position = glm::vec2{100.f, 100.f};
    tf.rotation = 30.f;
    Thickness& tc = GameContext::Get().AddComponent<Thickness>(seg, 30.f); 

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(SKYBLUE);

        GameContext::Get().Update(GetFrameTime());
        EndDrawing();
    }
    CloseWindow();
    return 0;
}