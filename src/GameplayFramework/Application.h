//
// Created by yuyzc on 2025/6/29.
//

#pragma once
#include <memory>


namespace ShapeGame
{
    class GameInstance;

    class Application
    {
    public:
        Application() = default;

        void Initialize();
        void Run();
        void Shutdown();

    private:
        void Tick();

    private:
        std::unique_ptr<GameInstance> TheInstance;
        bool bIsRunning = true;
    };
}
