//
// Created by yuyzc on 2025/7/27.
//
#include "Application.h"
#include "Core/Logger.h"
#include "Core/PlatformDefine.h"
#include <exception>

extern "C"
{
    using namespace ShapeEngine;
    SHAPE_ENGINE_API int EngineMain(int argc, char* argv[])
    {
        SetupLogger();

        if (argc < 2)
        {
            Logger()->critical("Usage: ShapeHost.exe <path-to-project.seproject> [-server]");
            return -1;
        }

        try
        {
            Application app;
            app.Initialize(argv[1]);

            app.Run();

            app.Shutdown();
        }
        catch (const std::exception& e)
        {
            Logger()->critical("An unhandled exception occurred in engine: {}", e.what());
            return -1;
        }
        ShutdownLogger();

        return 0;
    }
}