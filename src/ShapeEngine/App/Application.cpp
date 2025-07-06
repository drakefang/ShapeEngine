//
// Created by yuyzc on 2025/6/29.
//

#include "Application.h"
#include "Core/Logger.h"


namespace ShapeEngine
{
    void Application::Initialize()
    {
        ShapeEngine::SetupLogger();
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
}