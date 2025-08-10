//
// Created by yuyzc on 2025/8/10.
//

#pragma once
#include "Application.h"
#include "IPlugin.h"
#include "Core/Logger.h"
#include "Core/EngineTypes.h"
#include "Module/ModuleManager.h"
#include "Module/PluginModuleRegistry.h"


namespace ShapeEngine
{
    template<typename PluginTag>
    class PluginBase : public IPlugin
    {
    public:
        void Startup(const PluginDescriptor& descriptor) override
        {
            Logger()->info("'{}' plugin starting up and registering modules...", descriptor.FriendlyName);

            for (const auto& moduleDesc : descriptor.Modules)
            {
                // 从与 PluginTag 关联的、插件本地的工厂中获取创建方法
                if (auto factory = PluginModuleRegistry<PluginTag>::Get().GetFactory(moduleDesc.Name))
                {
                    // 将工厂函数注册给全局的 ModuleManager
                    ModuleManager::Get().RegisterModule(moduleDesc.Name, factory);
                    Logger()->info("Module '{}' registered by plugin '{}'.", moduleDesc.Name, descriptor.FriendlyName);
                }
                else
                {
                    Logger()->warn("Plugin '{}' failed to create module '{}'. Is it registered in its cpp file with the correct tag?", descriptor.FriendlyName, moduleDesc.Name);
                }
            }
        }

        void Shutdown() override
        {
            // Logger()->info("Plugin shutting down...");
        }
    };
}