//
// Created by yuyzc on 2025/6/29.
//

#pragma once
#include <memory>
#include <vector>

#include "DynamicLibrary.h"
#include "IPlugin.h"
#include "Core/PlatformDefine.h"

namespace ShapeEngine
{
    class IPrimaryGameModule;

    class SHAPE_ENGINE_API Application
    {
    public:
        Application();
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        void Initialize(const std::filesystem::path& projectFilePath);
        void Run();
        void Shutdown();

    private:
        void Tick();
        void LoadPlugins(const std::filesystem::path& projectRoot);
        void UnloadPlugins();

    private:
        bool bIsRunning = true;

        std::shared_ptr<IPrimaryGameModule> PrimaryGameModule = nullptr;

        std::vector<std::unique_ptr<IPlugin>> Plugins;
        std::vector<DynamicLibrary> LoadedLibraries;
    };
}
