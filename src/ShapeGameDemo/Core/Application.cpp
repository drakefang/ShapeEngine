//
// Created by yuyzc on 2025/8/4.
//

#include "Application.h"
#include "Log.h"
#include "Platform.h"
#include <SDL3/SDL.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <stdexcept>

#include "Components/TransformComponent.h"
#include "Systems/RenderSystem.h"

namespace ShapeGame
{
    bgfx::PlatformData GetPlatformData(SDL_Window* window) {
        bgfx::PlatformData pd;
        SDL_PropertiesID props = SDL_GetWindowProperties(window);

#if defined(SDL_PLATFORM_WIN32)
        pd.nwh = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
#elif defined(SDL_PLATFORM_MACOS)
        pd.nwh = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
#elif defined(SDL_PLATFORM_LINUX)
        if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0)
        {
            pd.ndt = SDL_GetPointerProperty(SDL_GetWindowProperties(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
            pd.nwh = SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
        }
#else
#error "Unsupported Platform"
#endif
        SDL_DestroyProperties(props);
        return pd;
    }

    Application::Application()
    {
        Init();
    }

    Application::~Application()
    {
        Shutdown();
    }

    void Application::Init()
    {
        SG_INFO("Application initializing...");
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            throw std::runtime_error("Failed to initialize SDL.");
        }

        m_Window = SDL_CreateWindow("ShapeGame", m_Width, m_Height, SDL_WINDOW_RESIZABLE);
        if (!m_Window)
        {
            throw std::runtime_error("Failed to create SDL window.");
        }

        bgfx::Init init;
        init.platformData = GetPlatformData(m_Window);
        init.resolution.width = m_Width;
        init.resolution.height = m_Height;
        init.resolution.reset = BGFX_RESET_VSYNC;

        if (!bgfx::init(init))
        {
            throw std::runtime_error("Failed to initialize BGFX.");
        }

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x1E1E1EFF, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, m_Width, m_Height);

        m_Renderer = std::make_unique<Renderer2D>();
        m_Renderer->Init();

        auto player = m_Registry.create();
        m_Registry.emplace<TransformComponent>(player,
            glm::vec2{0.0f, 0.0f}, // position
            0.0f,                  // orientation
            glm::vec2{50.0f, 50.0f}  // scale
        );

        auto& renderComp = m_Registry.emplace<RenderComponent>(player);
        Geometry drifterShape;
        drifterShape.vertices = {
            { 0.0f,  0.75f}, // Top
            {-0.5f, -0.5f}, // Bottom-left
            { 0.5f, -0.5f}  // Bottom-right
        };
        drifterShape.color = 0xFFFFFFFF; // White
        renderComp.geometries.push_back(drifterShape);


        m_LastTime = SDL_GetPerformanceCounter();
        SG_INFO("Application initialized successfully.");
    }

    void Application::Run()
    {
        while (m_IsRunning)
        {
            GameLoop();
        }
    }

    void Application::GameLoop()
    {
        uint64_t now = SDL_GetPerformanceCounter();
        auto deltaTime = static_cast<float>((now - m_LastTime) * 1.0 / (double)SDL_GetPerformanceFrequency());
        m_LastTime = now;

        HandleEvents();
        Update(deltaTime);
        Render();
    }

    void Application::HandleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                m_IsRunning = false;
            }
            if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                m_Width = event.window.data1;
                m_Height = event.window.data2;
                bgfx::reset(m_Width, m_Height, BGFX_RESET_VSYNC);
                bgfx::setViewRect(0, 0, 0, m_Width, m_Height);
            }
        }
    }

    void Application::Update(float deltaTime)
    {
        // Game logic systems update here.
    }

    void Application::Render()
    {
        bgfx::touch(0);

        auto view = glm::mat4(1.0f);
        glm::mat4 proj = glm::ortho(
            -m_Width / 2.0f, m_Width / 2.0f, // left, right
            -m_Height / 2.0f, m_Height / 2.0f, // bottom, top
            -1.0f, 1.0f // near, far
        );

        m_Renderer->BeginScene(proj * view);

        RenderSystem::Update(m_Registry, *m_Renderer);

        m_Renderer->EndScene();

        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 1, 0x4f, "ShapeGame Engine Skeleton");
        bgfx::dbgTextPrintf(
            0,
            2,
            0x6f,
            "FPS: %.2f",
            1.0f / (float)((SDL_GetPerformanceCounter() - m_LastTime) * 1.0 / (double)SDL_GetPerformanceFrequency()));

        bgfx::frame();
    }

    void Application::Shutdown()
    {
        SG_INFO("Application shutting down...");
        m_Renderer->Shutdown();
        bgfx::shutdown();
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
    }
}