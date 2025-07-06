#pragma once

#include <memory>
#include <spdlog/logger.h>

#include "PlatformDefine.h"

namespace ShapeEngine
{
    void SE_API SetupLogger();
    SE_API std::shared_ptr<spdlog::logger> Logger();
    void SE_API ShutdownLogger();
}