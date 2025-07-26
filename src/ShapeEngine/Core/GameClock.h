//
// Created by yuyzc on 2025/7/26.
//

#pragma once

#include <chrono>

#include "Core/GameTime.h"
#include "Core/PlatformDefine.h"

namespace ShapeEngine
{
    class SHAPE_ENGINE_API GameClock
    {
    public:
        GameClock();

        void Tick();
        const GameTime& GetCurrentTime() const { return CurrentTime; }

        void SetGlobalTimeDilation(float dilation);
        float GetGlobalTimeDilation() const { return GlobalTimeDilation; }
        bool IsPaused() const { return GlobalTimeDilation == 0.0f; }

    private:
        GameTime CurrentTime;
        float GlobalTimeDilation = 1.0f;
        std::chrono::time_point<std::chrono::high_resolution_clock> LastFrameTime;
    };
}
