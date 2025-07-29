//
// Created by yuyzc on 2025/7/27.
//
#include "Application.h"
#include "Core/Logger.h"
#include "Core/PlatformDefine.h"
#include <exception>
#include <filesystem>

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h> // For readlink on Linux
#include <limits.h> // For PATH_MAX
#if defined(__APPLE__)
#include <mach-o/dyld.h> // For _NSGetExecutablePath on macOS
#endif
#endif

namespace fs = std::filesystem;

// 跨平台地获取当前可执行文件的完整路径
fs::path GetExecutablePath()
{
#if defined(_WIN32)
    wchar_t path[MAX_PATH] = {0};
    GetModuleFileNameW(nullptr, path, MAX_PATH);
    return fs::path(path);
#elif defined(__linux__)
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return fs::path(std::string(result, (count > 0) ? count : 0));
#elif defined(__APPLE__)
    char path[PATH_MAX];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0)
        return fs::path(path);
    else
        return ""; // Should not happen
#else
    return ""; // Unsupported platform
#endif
}

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
        fs::path userProvidedPath = argv[1];
        fs::path projectFilePath;
        if (userProvidedPath.is_absolute())
        {
            projectFilePath = userProvidedPath;
        }
        else
        {
            fs::path exeDir = GetExecutablePath().parent_path();
            projectFilePath = exeDir / userProvidedPath;
        }
        projectFilePath = fs::weakly_canonical(projectFilePath);


        try
        {
            Application app;
            app.Initialize(projectFilePath);
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