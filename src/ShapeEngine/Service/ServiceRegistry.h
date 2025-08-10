//
// Created by yuyzc on 2025/8/10.
//

#pragma once

#include "Core/PlatformDefine.h"
#include <entt/entt.hpp>
#include <string>
#include <functional>
#include <memory>
#include <unordered_map>

#include "ServiceLocator.h"
#include "Module/ModuleManager.h"

namespace ShapeEngine
{
    class IModuleInterface;

    class SHAPE_ENGINE_API ServiceRegistry
    {
    public:
        static ServiceRegistry& Get();

        /**
         * @brief 注册一个服务接口的元信息和其注册动作。
         * @tparam T 服务的接口类型。
         */
        template<typename T>
        void RegisterInterface();

        /**
         * @brief 根据服务名，尝试将一个模块实例注册到 ServiceLocator。
         * @param serviceName 服务接口的名称（来自 .seplugin 文件）。
         * @param moduleInstance 已加载的模块实例。
         */
        void Provide(const std::string& serviceName, std::shared_ptr<IModuleInterface> moduleInstance);

    private:
        ServiceRegistry() = default;

        using RegistrationAction = std::function<void(std::shared_ptr<IModuleInterface>)>;

        std::unordered_map<entt::id_type, RegistrationAction> RegistrationActions;
    };


    // --- Template Implementation ---
    template<typename T>
    void ServiceRegistry::RegisterInterface()
    {
        // 1. 确保类型 T 的元数据被创建
        auto meta_type = entt::meta_factory<T>().type(entt::type_hash<T>::value());

        // 2. 创建一个“注册动作” lambda
        RegistrationAction action = [](std::shared_ptr<IModuleInterface> module)
        {
            if (auto service = std::dynamic_pointer_cast<T>(module))
            {
                ServiceLocator::Provide(service);
            }
        };

        // 3. 将这个动作与类型ID关联起来
        RegistrationActions[entt::type_hash<T>::value()] = action;
    }
}
