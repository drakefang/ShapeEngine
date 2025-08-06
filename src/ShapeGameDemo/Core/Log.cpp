//
// Created by yuyzc on 2025/8/4.
//

#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace ShapeGame
{
    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_CoreLogger = spdlog::stdout_color_mt("ShapeGame");
        s_CoreLogger->set_level(spdlog::level::trace);
        SG_INFO("Initialized Log System");
    }
}