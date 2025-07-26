//
// Created by yuyzc on 2025/7/26.
//

#include "GameClock.h"


namespace ShapeEngine
{
    GameClock::GameClock()
    {
        LastFrameTime = std::chrono::high_resolution_clock::now();
    }

    void GameClock::Tick()
    {
        const auto currentTime = std::chrono::high_resolution_clock::now();

        CurrentTime.UnscaledDeltaTime = std::chrono::duration<float>(currentTime - LastFrameTime).count();
        LastFrameTime = currentTime;

        CurrentTime.DeltaTime = CurrentTime.UnscaledDeltaTime * GlobalTimeDilation;

        CurrentTime.TotalUnscaledTime += CurrentTime.UnscaledDeltaTime;
        CurrentTime.TotalTime += CurrentTime.DeltaTime;
    }

    void GameClock::SetGlobalTimeDilation(float dilation)
    {
        GlobalTimeDilation = std::max(0.0f, dilation);
    }
}