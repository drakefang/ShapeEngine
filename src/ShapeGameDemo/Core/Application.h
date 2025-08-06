//
// Created by yuyzc on 2025/8/4.
//

#pragma once


#include <entt/entt.hpp>
#include "Renderer/Renderer2D.h"

struct SDL_Window;

namespace ShapeGame
{
    class Application
    {
    public:
        Application();
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        void Run();

    private:
        void Init();
        void Shutdown();
        void GameLoop();

        void HandleEvents();
        void Update(float deltaTime);
        void Render();

        bool m_IsRunning = true;
        SDL_Window* m_Window = nullptr;
        int m_Width = 1280;
        int m_Height = 720;

        entt::registry m_Registry;
        entt::dispatcher m_Dispatcher;
        std::unique_ptr<Renderer2D> m_Renderer;

        uint64_t m_LastTime = 0;
    };
}