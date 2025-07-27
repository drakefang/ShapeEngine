#pragma once
#include <memory>
#include <spdlog/logger.h>

namespace ShapeGame
{
    void SetupLogger();
    std::shared_ptr<spdlog::logger> Logger();
    void ShutdownLogger();
}