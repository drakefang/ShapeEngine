#pragma once

#include <memory>
#include <spdlog/logger.h>

#include "PlatformDefine.h"

namespace ShapeEngine
{
    void SHAPE_ENGINE_API SetupLogger();
    SHAPE_ENGINE_API std::shared_ptr<spdlog::logger> Logger();
    void SHAPE_ENGINE_API ShutdownLogger();
}