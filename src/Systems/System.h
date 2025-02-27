
#pragma once

#include "entt/entt.hpp"

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
    };
} // namespace ShapeGame