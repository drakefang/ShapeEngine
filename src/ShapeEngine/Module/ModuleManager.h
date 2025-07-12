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

#include "Core/PlatformDefine.h"

namespace ShapeEngine
{
    class IModule;
    class IModuleInterface;
    using ModuleFactory = std::function<std::unique_ptr<IModule>()>;

    class SE_API ModuleManager
    {
    public:
        static ModuleManager& Get();

        ModuleManager(const ModuleManager&) = delete;
        ModuleManager& operator=(const ModuleManager&) = delete;

        void RegisterModule(const std::string &moduleName, const ModuleFactory &factory);

        IModuleInterface& LoadModule(const std::string& moduleName);

        bool IsModuleLoaded(const std::string& moduleName) const;

        void ShutdownAllModules();

    private:
        ModuleManager() = default;
        ~ModuleManager() = default;

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

        std::map<std::string, ModuleInfo> m_RegisteredModules;
        std::map<std::string, std::unique_ptr<IModule>> m_LoadedModules;
        std::vector<std::string> m_StartupOrder;
    };

    template<typename T>
    T& LoadModuleChecked(const std::string& moduleName)
    {
        static_assert(std::is_base_of_v<IModuleInterface, T>, "T must be a module interface.");
        IModuleInterface& module = ModuleManager::Get().LoadModule(moduleName);
        T* result = dynamic_cast<T*>(&module);
        if (!result)
        {
            throw std::runtime_error("Failed to cast module '" + moduleName + "' to the requested interface type.");
        }
        return *result;
    }
}