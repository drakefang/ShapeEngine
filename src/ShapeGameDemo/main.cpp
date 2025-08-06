//
// Created by yuyzc on 2025/8/3.
//

#include "Core/Application.h"
#include "Core/Log.h"
#include <memory>
#include <exception>

int main(int, char**)
{
    ShapeGame::Log::Init();

    try
    {
        auto app = std::make_unique<ShapeGame::Application>();
        app->Run();
    }
    catch (const std::exception& e)
    {
        SG_CRITICAL("Main loop terminated with an exception: {0}", e.what());
        return 1;
    }

    SG_INFO("Shutting down application.");
    return 0;
}