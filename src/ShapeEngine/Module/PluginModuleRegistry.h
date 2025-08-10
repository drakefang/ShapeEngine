//
// Created by yuyzc on 2025/8/10.
//

#pragma once
#include <memory>
#include <string>
#include <functional>
#include <map>
#include "Core/EngineTypes.h"

namespace ShapeEngine
{
    class IModule;

    template<typename PluginTag>
    class PluginModuleRegistry
    {
    public:
        static PluginModuleRegistry& Get()
        {
            static PluginModuleRegistry instance;
            return instance;
        }

        void Register(const std::string& name, ModuleFactoryFunc factory)
        {
            Factories[name] = factory;
        }

        ModuleFactoryFunc GetFactory(const std::string& name) const
        {
            auto it = Factories.find(name);
            return (it != Factories.end()) ? it->second : nullptr;
        }

    private:
        PluginModuleRegistry() = default;
        std::map<std::string, ModuleFactoryFunc> Factories;
    };

    template<typename ModuleType, typename PluginTag>
    class ScopedModuleRegistrar
    {
    public:
        ScopedModuleRegistrar(const std::string& name)
        {
            PluginModuleRegistry<PluginTag>::Get().Register(name, []() {
                return std::make_shared<ModuleType>();
            });
        }
    };
}