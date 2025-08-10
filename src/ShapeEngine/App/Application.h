//
// Created by yuyzc on 2025/6/29.
//

#pragma once
#include <memory>
#include <vector>

#include "DynamicLibrary.h"
#include "IPlugin.h"
#include "Core/PlatformDefine.h"
#include "Core/SubsystemManager.h"
#include "Core/EngineTypes.h"

namespace ShapeEngine
{
    class IPrimaryGameModule;

    struct ModuleDescriptor
    {
        std::string Name;
        std::string Library;
        std::string Type;
        std::string ProvidesService;
        EModuleLoadingPhase LoadingPhase = EModuleLoadingPhase::None;
    };

    struct PluginDescriptor
    {
        std::filesystem::path FilePath;
        std::string FriendlyName;
        std::vector<ModuleDescriptor> Modules;
        bool bIsEnabled = false;
    };


    class SHAPE_ENGINE_API Application : public std::enable_shared_from_this<Application>
    {
    public:
        virtual ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        static std::shared_ptr<Application> Create();

        bool Initialize(const std::filesystem::path& projectFilePath);
        void Run();
        void Shutdown();

    protected:
        Application();

    private:
        void Tick();
        void LoadPlugins(const std::filesystem::path& projectRoot);
        void UnloadPlugins();

        void RegisterCoreSubsystems();
        void RegisterServiceInterfaces();
        void LoadAndRegisterService(const ModuleDescriptor& moduleDesc);

    private:
        bool bIsRunning = true;
        bool bIsInitialized = false;

        std::shared_ptr<IPrimaryGameModule> PrimaryGameModule = nullptr;

        std::vector<std::unique_ptr<IPlugin>> Plugins;
        std::vector<DynamicLibrary> LoadedLibraries;

        SubsystemManager SubsystemManager;

        std::vector<PluginDescriptor> FoundPlugins;
    };
}
