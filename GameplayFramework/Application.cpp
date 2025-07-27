//
// Created by yuyzc on 2025/6/29.
//

#include "Application.h"
#include "Core/MetaRegistry.h"

#include "GameInstance.h"
#include "Core/Logger.h"


namespace ShapeGame
{
    void Application::Initialize()
    {
        ShapeGame::SetupLogger();
        ShapeGame::MetaRegistry::RegisterAll();

        MetaRegistry::RegisterAll();

        TheInstance = std::make_unique<GameInstance>();
        TheInstance->Initialize();
        TheInstance->LoadWorld("DefaultMap");
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
        TheInstance->Shutdown();
        TheInstance = nullptr;

        ShapeGame::ShutdownLogger();
    }

    void Application::Tick()
    {
        float deltaTime = 0.f;

        TheInstance->Tick(deltaTime);
    }
}