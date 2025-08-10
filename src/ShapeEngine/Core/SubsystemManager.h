//
// Created by yuyzc on 2025/8/10.
//

#pragma once

#include <memory>
#include <unordered_map>

#include "PlatformDefine.h"
#include "ISubsystem.h"
#include <vector>

namespace ShapeEngine
{
    class SHAPE_ENGINE_API SubsystemManager
    {
    public:
        SubsystemManager() = default;
        ~SubsystemManager();

        SubsystemManager(const SubsystemManager&) = delete;
        SubsystemManager& operator=(const SubsystemManager&) = delete;

        template<typename T, typename... Args>
        std::shared_ptr<T> RegisterSubsystem(Args&&... args)
        {
            static_assert(std::is_base_of_v<ISubSystem, T>, "Subsystem must inherit from ISubsystem.");

            auto newSubsystem = std::make_shared<T>(std::forward<Args>(args)...);
            const char* name = newSubsystem->GetName();
            if (SubsystemMap.contains(name))
            {
                return nullptr;
            }

            SubsystemMap[name] = newSubsystem;
            Subsystems.push_back(newSubsystem);
            return newSubsystem;
        }

        void StartupAll();

        void ShutdownAll();

        template<typename T>
        std::shared_ptr<T> GetSubsystem() const
        {
            static_assert(std::is_base_of_v<ISubSystem, T>, "Subsystem must inherit from ISubsystem.");

            T temp;
            const char* name = temp.GetName();

            if (const auto it = SubsystemMap.find(name); it != SubsystemMap.end())
            {
                return std::static_pointer_cast<T>(it->second);
            }
            return nullptr;
        }

    private:
        enum class EVisitState { Unvisited, Visiting, Visited };

        static void TopologicalSort(const std::shared_ptr<ISubSystem>& system,
                                    std::vector<ISubSystem*>& sortedList,
                                    std::unordered_map<std::string, EVisitState>& visitState,
                                    std::unordered_map<std::string, std::shared_ptr<ISubSystem>>& systemMap);

        std::vector<std::shared_ptr<ISubSystem>> Subsystems;
        std::unordered_map<std::string, std::shared_ptr<ISubSystem>> SubsystemMap;
        std::vector<ISubSystem*> StartupOrder;
    };
}
