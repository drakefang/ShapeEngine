

#pragma once

#include "SystemBase.h"
#include "entt/entity/registry.hpp"
#include "entt/entity/fwd.hpp"
#include <memory>
#include <utility>
#include <vector>

namespace ShapeGame
{
    class GameContext
    {
    public:
        static GameContext& Get();

        entt::registry& Registry()
        {
            return registry;
        }

        // Systems
        template<class T, typename... Args>
        void RegisterSystem(Args&& ... args)
        {
            systems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

        void Initialize();
        void Update(float DeltaTime);

        // Entities
        entt::entity CreateEntity();
        template<class T, typename... Args>
        T& AddComponent(entt::entity e, Args&&... args)
        {
            return Registry().emplace<T>(e, std::forward<Args>(args)...);
        }

        template<class T>
        void RemoveComponent(entt::entity e)
        {
            if (Registry().any_of<T>())
            {
                Registry().remove<T>(e);
            }
        }

    private:
        entt::registry registry;

        std::vector<std::unique_ptr<ISystem>> systems;
    };
} // namespace ShapeGame