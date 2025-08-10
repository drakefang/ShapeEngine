//
// Created by yuyzc on 2025/7/8.
//

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "Core/ISubsystem.h"
#include "Core/PlatformDefine.h"

namespace ShapeEngine
{
    class IModule;
    class IModuleInterface;
    using ModuleFactory = std::function<std::shared_ptr<IModule>()>;

    class SHAPE_ENGINE_API ModuleManager final : public IEngineSubSystem
    {
    public:
        static ModuleManager& Get();

        ModuleManager() = default;
        ~ModuleManager() = default;
        ModuleManager(const ModuleManager&) = delete;
        ModuleManager& operator=(const ModuleManager&) = delete;

        void RegisterModule(const std::string &moduleName, const ModuleFactory &factory);

        std::shared_ptr<IModuleInterface> LoadModule(const std::string& moduleName);

        [[nodiscard]] bool IsModuleLoaded(const std::string& moduleName) const;

        void ShutdownAllModules();

        virtual const char* GetName() const noexcept override;

    private:
        // 拓扑排序算法，用于计算正确的加载顺序
        enum class EVisitState { Unvisited, Visiting, Visited };
        void TopologicalSort(const std::string& moduleName,
                             std::vector<std::string>& sortedList,
                             std::map<std::string, EVisitState>& visitState);

        // 存储已注册但未加载的模块信息
        struct ModuleInfo
        {
            ModuleFactory Factory;
            std::vector<std::string> Dependencies;
        };

        std::map<std::string, ModuleInfo> RegisteredModules;
        std::map<std::string, std::shared_ptr<IModule>> LoadedModules;
        std::vector<std::string> StartupOrder;
    };

    template<typename T>
    std::shared_ptr<T> LoadModuleChecked(const std::string& moduleName)
    {
        static_assert(std::is_base_of_v<IModuleInterface, T>, "T must be a module interface.");
        const auto module = ModuleManager::Get().LoadModule(moduleName);
        auto result = std::dynamic_pointer_cast<T>(module);
        if (!result)
        {
            throw std::runtime_error("Failed to cast module '" + moduleName + "' to the requested interface type.");
        }
        return result;
    }
}