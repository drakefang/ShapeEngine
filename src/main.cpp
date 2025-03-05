
#include "Core/BaseComponents.h"
#include "Core/GameContext.h"
#include "Core/ShapeComponents.h"
#include "entt/entt.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_uint4_sized.hpp"
#include "raylib.h"

#include "Core/Renderer/RenderSystem.h"

using namespace ShapeGame;

int main(int argc, char** argv)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(1024, 768, "ShapeGame");
    SetTargetFPS(60);

    GameContext::Get().Initialize();

    auto seg = GameContext::Get().CreateEntity();
    LineSegment& sc = GameContext::Get().AddComponent<LineSegment>(seg);
    sc.left = glm::vec2{-100.f, 0};
    sc.right = glm::vec2{100.f, 0};
    sc.color = glm::u8vec4{255, 0, 0, 255};
    ShapeGame::Transform& tf = GameContext::Get().AddComponent<ShapeGame::Transform>(seg);
    tf.position = glm::vec2{100.f, 100.f};
    tf.rotation = 0.f;
    Thickness& tc = GameContext::Get().AddComponent<Thickness>(seg, 10.f); 
    GameContext::Get().AddComponent<Visible>(seg, true);
    GameContext::Get().AddComponent<RoundedCap>(seg, 16);

    auto triangle = GameContext::Get().CreateEntity();
    GameContext::Get().AddComponent<Visible>(triangle, true);
    GameContext::Get().AddComponent<Filled>(triangle, false);
    GameContext::Get().AddComponent<Thickness>(triangle, 10.f);
    Polygon& p = GameContext::Get().AddComponent<Polygon>(triangle);
    p.vertices = {
        glm::vec2{0, -50.f},
        glm::vec2{-100, 30.f},
        glm::vec2{100, 30.f},
    };
    p.color = glm::u8vec4{0, 255, 0, 255};
    ShapeGame::Transform& ttf = GameContext::Get().AddComponent<ShapeGame::Transform>(triangle);
    ttf.position = glm::vec2{-100.f, 100.f};
    ttf.rotation = -30.f;
    GameContext::Get().AddComponent<RoundedCap>(triangle, 16);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(SKYBLUE);


        //ttf.rotation += 30.f * GetFrameTime();

        GameContext::Get().Update(GetFrameTime());
        EndDrawing();
    }
    CloseWindow();
    return 0;
}