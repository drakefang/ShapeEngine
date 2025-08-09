//
// Created by yuyzc on 2025/6/29.
//

#include "Application.h"

#include <fstream>

#include "ConfigManager.h"
#include "IPrimaryGameModule.h"
#include "Core/Logger.h"
#include "Module/ModuleManager.h"

#include <toml++/toml.h>

#include "Service/ServiceLocator.h"

namespace ShapeEngine
{
    void CreateDefaultProjectFile(const std::filesystem::path& projectFilePath)
    {
        Logger()->info("Project file not found. Creating a default one at: {}", projectFilePath.string());

        // 从路径中提取项目名称作为默认模块名
        std::string defaultModuleName = projectFilePath.stem().string();

        std::ofstream projectFile(projectFilePath);
        if (projectFile.is_open())
        {
            projectFile << R"(# ShapeEngine Project Descriptor - Auto-generated
# This file describes your game project to the engine.

FormatVersion = "1.0"

# [Required] The primary game module to load on startup.
# This should match the name of your game's main plugin module.
PrimaryGameModuleName = ")" << defaultModuleName << R"("

# [Recommended] A list of game-specific configuration files to load.
# These files will override settings from the engine's DefaultEngine.toml.
GameConfigFiles = [
    "Config/Game.toml",
]
)";
            projectFile.close();
        }
        else
        {
            Logger()->error("Failed to create default project file at: {}", projectFilePath.string());
        }
    }

    void CreateDefaultGameConfigFile(const std::filesystem::path& configFilePath)
    {
        Logger()->info("Game config file not found. Creating a default one at: {}", configFilePath.string());

        // 确保 Config/ 目录存在
        std::filesystem::create_directories(configFilePath.parent_path());

        std::ofstream configFile(configFilePath);
        if (configFile.is_open())
        {
            configFile << R"(# MyGame - Default Game Configuration
# This file overrides settings from the engine's DefaultEngine.toml.

[Platform.Window]
Title = "My Awesome ShapeEngine Game!"
Width = 1280
Height = 720
)";
            configFile.close();
        }
        else
        {
            Logger()->error("Failed to create default game config file at: {}", configFilePath.string());
        }
    }

    Application::Application()
    {
    }

    Application::~Application()
    {
        if (bIsRunning)
        {
            Shutdown();
        }
    }

    std::shared_ptr<Application> Application::Create()
    {
        return std::make_shared<Application>();
    }

    void Application::Initialize(const std::filesystem::path& projectFilePath)
    {
        Logger()->info("==================================================");
        Logger()->info("Application Initializing...");
        Logger()->info("==================================================");

        std::shared_ptr<GameClock> clockPtr(&AppGameClock, [](GameClock*){});
        ServiceLocator::Provide(clockPtr);

        std::shared_ptr<TimerManager> timerPtr(&AppTimerManager, [](TimerManager*){});
        ServiceLocator::Provide(timerPtr);

        ServiceLocator::Provide(shared_from_this());

        if (!std::filesystem::exists(projectFilePath))
        {
            CreateDefaultProjectFile(projectFilePath);
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
        auto projectRoot = projectFilePath.parent_path();
        configFilesToLoad.emplace_back("Config/DefaultEngine.toml");
        if (const auto gameConfigNode = projectDesc["GameConfigFiles"].as_array())
        {
            for (const auto& element : *gameConfigNode)
            {
                if (auto pathStr = element.value<std::string>())
                {
                    auto gameConfigPath = projectRoot / *pathStr;

                    if (!std::filesystem::exists(gameConfigPath))
                    {
                        CreateDefaultGameConfigFile(gameConfigPath);
                    }

                    configFilesToLoad.push_back(gameConfigPath);
                }
            }
        }
        ConfigManager::Get().LoadFromFiles(configFilesToLoad);
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
    }

    void Application::Tick()
    {
        AppGameClock.Tick();
        const GameTime& gameTime = AppGameClock.GetCurrentTime();

        AppTimerManager.Tick(gameTime);

        if (PrimaryGameModule)
        {
            PrimaryGameModule->Tick(gameTime);
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