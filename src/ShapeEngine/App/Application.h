//
// Created by yuyzc on 2025/6/29.
//

#pragma once
#include <memory>
#include "Core/PlatformDefine.h"


namespace ShapeEngine
{
    class SE_API Application
    {
    public:
        Application() = default;

        void Initialize();
        void Run();
        void Shutdown();

    private:
        void Tick();

    private:
        bool bIsRunning = true;
    };
}
