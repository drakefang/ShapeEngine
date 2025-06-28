
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>

#include "Core/Logger.h"
#include "entt/entity/registry.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "Core/MetaRegistry.h"

int main(int argc, char** argv)
{
    ShapeGame::SetupLogger();
    ShapeGame::MetaRegistry::RegisterAll();


    ShapeGame::ShutdownLogger();
    return 0;
}