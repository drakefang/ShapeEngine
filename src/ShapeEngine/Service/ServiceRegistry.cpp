//
// Created by yuyzc on 2025/8/10.
//

#include "ServiceRegistry.h"

#include "Core/Logger.h"

namespace ShapeEngine
{
    ServiceRegistry& ServiceRegistry::Get()
    {
        static ServiceRegistry instance;
        return instance;
    }

    void ServiceRegistry::Provide(const std::string& serviceName, std::shared_ptr<IModuleInterface> moduleInstance)
    {
        if (!moduleInstance)
        {
            return;
        }
        // 1. 从字符串名称解析出元类型
        auto meta_type = entt::resolve(entt::hashed_string(serviceName.c_str()));
        if (!meta_type)
        {
            Logger()->error("{} is not found!", serviceName);
            return;
        }

        // 2. 使用元类型的ID去查找我们之前存储的“注册动作”
        auto it = RegistrationActions.find(meta_type.id());
        if (it != RegistrationActions.end())
        {
            // 3. 执行这个动作！
            it->second(moduleInstance);
        }
        else
        {
            Logger()->error("{} is not registered!", serviceName);
        }
    }
} // ShapeEngine