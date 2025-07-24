//
// Created by yuyzc on 2025/6/29.
//

#include "Application.h"
#include "Core/Logger.h"


namespace ShapeEngine
{
    Application::Application()
    {
        SetupLogger();
    }

    Application::~Application()
    {
        ShutdownLogger();

        if (bIsRunning)
        {
            Shutdown();
        }
    }

    void Application::Initialize()
    {
        Logger()->info("==================================================");
        Logger()->info("Application Initializing...");
        Logger()->info("==================================================");

        LoadPlugins();
    }

    void Application::Run()
    {
        while (bIsRunning)
        {
            Tick();
        }
    }

    void Application::Shutdown()
    {

        ShapeEngine::ShutdownLogger();
    }

    void Application::Tick()
    {
        float deltaTime = 0.f;

    }

    void Application::LoadPlugins()
    {
    }

    void Application::UnloadPlugins()
    {
    }
}