//
// Created by yuyzc on 2025/6/29.
//

#include "Application.h"

#include "ConfigManager.h"
#include "IPrimaryGameModule.h"
#include "Core/Logger.h"
#include "Module/ModuleManager.h"

#include <toml++/toml.h>

namespace ShapeEngine
{
    Application::Application()
    {
        SetupLogger();
    }

    Application::~Application()
    {
        ShutdownLogger();

        if (bIsRunning)
        {
            Shutdown();
        }
    }

    void Application::Initialize(const std::filesystem::path& projectFilePath)
    {
        Logger()->info("==================================================");
        Logger()->info("Application Initializing...");
        Logger()->info("==================================================");

        if (std::filesystem::exists(projectFilePath))
        {
            throw std::runtime_error("Project File not found:" + projectFilePath.string());
        }

        toml::table projectDesc;
        try
        {
            projectDesc = toml::parse_file(projectFilePath.string());
        }
        catch (const toml::parse_error& err)
        {
            const std::string errMsg = "Failed to parse TOML file:'" + projectFilePath.string() + "':\n" + err.what();
            throw std::runtime_error(errMsg);
        }
        std::vector<std::filesystem::path> configFilesToLoad;
        configFilesToLoad.push_back("Config/DefaultEngine.ini");
        if (const auto gameConfigNode = projectDesc["GameConfigFiles"].as_array())
        {
            for (const auto& element : *gameConfigNode)
            {
                if (auto pathStr = element.value<std::string>())
                {
                    configFilesToLoad.push_back(*pathStr);
                }
            }
        }
        ConfigManager::Get().LoadFromFiles(configFilesToLoad);
        const std::filesystem::path projectRoot = projectFilePath.parent_path();
        LoadPlugins(projectRoot);

        auto primaryGameModuleName = projectDesc["PrimaryGameModuleName"].value_or<std::string>("");
        if (primaryGameModuleName.empty())
        {
            primaryGameModuleName = ConfigManager::Get().GetValueOrDefault<std::string>("Engine.PrimaryGameModule", "");
        }
        if (primaryGameModuleName.empty())
        {
            throw std::runtime_error("PrimaryGameModuleName not specified in project file or config.");
        }

        Logger()->info("Loading primary game module: {}", primaryGameModuleName);
        PrimaryGameModule = LoadModuleChecked<IPrimaryGameModule>(primaryGameModuleName);

        bIsRunning = true;
        Logger()->info("Application initialization complete.");
    }

    void Application::Run()
    {
        while (bIsRunning)
        {
            Tick();
        }
    }

    void Application::Shutdown()
    {
        if (!bIsRunning) return;

        Logger()->info("==================================================");
        Logger()->info("Application Shutting Down...");
        Logger()->info("==================================================");

        ModuleManager::Get().ShutdownAllModules();
        UnloadPlugins();

        bIsRunning = false;
        Logger()->info("Application shutdown complete.");
        ShapeEngine::ShutdownLogger();
    }

    void Application::Tick()
    {
        float deltaTime = 0.f;

        if (PrimaryGameModule)
        {
            PrimaryGameModule->Tick(deltaTime);
        }
    }

    void Application::LoadPlugins(const std::filesystem::path& projectRoot)
    {
        const std::filesystem::path pluginsPath = projectRoot / ConfigManager::Get().GetValueOrDefault<std::string>("Plugins.Path", "Plugins/");
        Logger()->info("Scanning for plugins in: {}", pluginsPath.string());

        if (!std::filesystem::exists(pluginsPath) || !std::filesystem::is_directory(pluginsPath))
        {
            Logger()->warn("Plugins directory not found: {}", pluginsPath.string());
            return;
        }

        using FCreatePluginFunc = IPlugin* (*)();

        for (const auto& entry : std::filesystem::directory_iterator(pluginsPath))
        {
            const auto& path = entry.path();
#if defined(_WIN32)
            const std::string extension = ".dll";
#elif defined(__APPLE__)
            const std::string extension = ".dylib";
#else
            const std::string extension = ".so";
#endif
            if (!entry.is_regular_file() || path.extension() != extension) continue;

            DynamicLibrary library(path);
            if (!library.IsValid()) continue;

            const auto createPluginFunc = reinterpret_cast<FCreatePluginFunc>(library.GetSymbol("CreatePlugin"));
            if (!createPluginFunc)
            {
                Logger()->warn("Plugin '{}' does not export a valid 'CreatePlugin' entry point. Skipping.", path.string());
                continue;
            }

            if (std::unique_ptr<IPlugin> plugin(createPluginFunc()); plugin)
            {
                Logger()->info("Plugin '{}' loaded successfully. Starting up...", path.filename().string());
                plugin->Startup();
                Plugins.push_back(std::move(plugin));
                LoadedLibraries.push_back(std::move(library));
            }
        }
    }

    void Application::UnloadPlugins()
    {
        for (auto it = Plugins.rbegin(); it != Plugins.rend(); ++it)
        {
            (*it)->Shutdown();
        }
        Plugins.clear();

        LoadedLibraries.clear();
        Logger()->info("All plugins have been shut down and unloaded.");
    }
}