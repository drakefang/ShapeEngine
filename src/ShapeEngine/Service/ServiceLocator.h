//
// Created by yuyzc on 2025/7/26.
//

#pragma once

#include <any>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

#include "Core/PlatformDefine.h"

namespace ShapeEngine
{
    class ServiceLocator
    {
    public:
        template<typename T>
        static void Provide(T* service)
        {
            Services[typeid(T)] = service;
        }

        template<typename T>
        static T& Get()
        {
            const auto it = Services.find(typeid(T));
            if (it == Services.end())
            {
                throw std::runtime_error("Service not found: " + std::string(typeid(T).name()));
            }

            try
            {
                return *std::any_cast<T*>(it->second);
            }
            catch (const std::bad_any_cast& e)
            {
                throw std::runtime_error("Service cast failed: " + std::string(e.what()));
            }
        }

    private:
        inline static std::unordered_map<std::type_index, std::any> Services;
    };
}
