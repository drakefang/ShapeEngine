
#pragma once


namespace ShapeGame
{
    struct Timer
    {
        float interval;
        float elapsed = 0.f;
        bool looping = false;
        bool paused = false;

        size_t handle = 0;
    };
}