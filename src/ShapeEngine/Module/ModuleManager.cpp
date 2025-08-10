//
// Created by yuyzc on 2025/7/8.
//

#include "ModuleManager.h"

#include "IModuleInterface.h"
#include "Core/Logger.h"


namespace ShapeEngine
{
    ModuleManager& ModuleManager::Get()
    {
        static ModuleManager instance;
        return instance;
    }

    void ModuleManager::RegisterModule(const std::string& moduleName, const ModuleFactoryFunc& factory)
    {
        if (RegisteredModules.contains(moduleName))
        {
            Logger()->warn("Module '{}' is already registered. Overwriting.", moduleName);
        }

        if (!factory)
        {
            Logger()->error("An empty factory was provided for module '{}'.", moduleName);
            return;
        }

        const std::shared_ptr<IModule> tempInstance(factory());
        if (!tempInstance)
        {
            Logger()->error("Module factory for '{}' returned a nullptr.", moduleName);
            return;
        }

        RegisteredModules[moduleName] = {factory, tempInstance->GetDependencies()};
        Logger()->info("Module factory registered: {}", moduleName);
    }

    std::shared_ptr<IModuleInterface> ModuleManager::LoadModule(const std::string& moduleName)
    {
        if (IsModuleLoaded(moduleName))
        {
            auto interface = std::dynamic_pointer_cast<IModuleInterface>(LoadedModules.at(moduleName));
            if (!interface)
            {
                throw std::runtime_error("Module '" + moduleName + "' was loaded but is not an interface module.");
            }
            return interface;
        }

        std::vector<std::string> loadOrder;
        std::map<std::string, EVisitState> visitState;
        TopologicalSort(moduleName, loadOrder, visitState);

        for (const auto& name : loadOrder)
        {
            if (!IsModuleLoaded(name))
            {
                Logger()->info("Loading module: {}", name);
                const auto& [Factory, Dependencies] = RegisteredModules.at(name);
                std::shared_ptr<IModule> moduleInstance(Factory());

                LoadedModules[name] = std::move(moduleInstance);
                StartupOrder.push_back(name);

                LoadedModules.at(name)->Startup();
            }
        }

        auto interface = std::dynamic_pointer_cast<IModuleInterface>(LoadedModules.at(moduleName));
        if (!interface)
        {
            throw std::runtime_error("Module '" + moduleName + "' was loaded but is not an interface module.");
        }
        return interface;
    }

    bool ModuleManager::IsModuleLoaded(const std::string& moduleName) const
    {
        return LoadedModules.contains(moduleName);
    }

    void ModuleManager::ShutdownAllModules()
    {
        Logger()->info("Shutting down all modules...");

        std::reverse(StartupOrder.begin(), StartupOrder.end());
        for (const auto& moduleName : StartupOrder)
        {
            if (LoadedModules.contains(moduleName))
            {
                Logger()->info("Shutting down module: {}", moduleName);
                LoadedModules.at(moduleName)->Shutdown();
            }
        }

        LoadedModules.clear();
        StartupOrder.clear();
    }

    const char* ModuleManager::GetName() const noexcept
    {
        return "ModuleManager";
    }

    void ModuleManager::TopologicalSort(
        const std::string& moduleName,
        std::vector<std::string>& sortedList,
        std::map<std::string, EVisitState>& visitState)
    {
        visitState[moduleName] = EVisitState::Visiting;

        if (!RegisteredModules.contains(moduleName))
        {
            throw std::runtime_error("Dependency module not found: " + moduleName);
        }

        for (const auto& dependencies = RegisteredModules.at(moduleName).Dependencies;
            const auto& depName : dependencies)
        {
            if (visitState[depName] == EVisitState::Unvisited)
            {
                TopologicalSort(depName, sortedList, visitState);
            }
            else if (visitState[depName] == EVisitState::Visiting)
            {
                const std::string msg = fmt::format("Circular dependency detected: {} depends on {}, which has a dependency back to it",
                    moduleName, depName);
                throw std::runtime_error(msg);
            }
        }

        visitState[moduleName] = EVisitState::Visited;
        sortedList.push_back(moduleName);
    }
}