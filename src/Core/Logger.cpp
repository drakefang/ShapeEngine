
#include "Logger.h"

#include <entt/locator/locator.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace ShapeGame
{
    using LoggerLocator = entt::locator<std::shared_ptr<spdlog::logger>>;
    void SetupLogger()
    {
        auto logger = spdlog::stdout_color_mt("ShapeGameLogger");
        logger->set_level(spdlog::level::debug);
        logger->info("Game Start!");
        LoggerLocator::emplace(logger);
    }

    std::shared_ptr<spdlog::logger> Logger()
    {
        if (auto logger = LoggerLocator::value())
        {
            return logger;
        }
        return nullptr;
    }

    void ShutdownLogger()
    {
        LoggerLocator::reset();

        spdlog::shutdown();
    }
}
