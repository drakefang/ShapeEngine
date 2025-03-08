
#include "BaseComponents.h"
#include "GameContext.h"
#include "ShapeComponents.h"
#include "Timer/Timer.h"
#include "Timer/TimerSystem.h"
#include "entt/entt.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_uint4_sized.hpp"
#include "raylib.h"

#include "Renderer/RenderSystem.h"
#include <iostream>

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
    tf.scale = glm::vec2{2.f, 2.f};
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
    ttf.rotation = 0.f;
    ttf.scale = glm::vec2{2.5f, 2.5f};
    GameContext::Get().AddComponent<RoundedCap>(triangle, 16);
    Timer& timer = GameContext::Get().AddComponent<Timer>(triangle, 1.f, 0.f, true, false);
    TimerSystem::BindCallback(timer, []() { std::cout << "timer without params!!!" << std::endl; });
    // entt 不允许给同一个entity添加相同类型component
    Timer& timer1 = GameContext::Get().AddComponent<Timer>(seg, 0.3f, 0.f, false, false);
    int tp = 5;
    TimerSystem::BindCallback(
        timer1,
        [tp](int p, const char* txt) { std::cout << "timer with param:" << tp + p << txt << std::endl; },
        4,
        "hello!!!");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(SKYBLUE);

        // ttf.rotation += 30.f * GetFrameTime();
        tf.rotation += 30.f * GetFrameTime();

        GameContext::Get().Update(GetFrameTime());
        EndDrawing();
    }
    CloseWindow();
    return 0;
}