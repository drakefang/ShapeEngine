
#pragma once

#include "entt/entity/fwd.hpp"
#include "entt/entt.hpp"
#include <memory>
#include <vector>

namespace ShapeGame
{
    class System
    {
    public:
        System(entt::registry& reg) : registry(reg) {}
        System(const System&) = default;
        System(System&&) = default;
        System& operator=(System&&) = delete;
        System& operator=(const System&) = delete;
        virtual ~System() noexcept = default;

        virtual void Update(float DeltaTime) = 0;

    protected:
        entt::registry& registry;

    public:
        template <class T> static void AddSystem(entt::registry& reg)
        {
            SubSystems.push_back(std::make_unique<T>(reg));
        }

        static void UpdateSystems(float DeltaTime)
        {
            for(auto& system : SubSystems)
            {
                system->Update(DeltaTime);
            }
        }

    private:
        static std::vector<std::unique_ptr<System>> SubSystems;
    };
} // namespace ShapeGame