
#pragma once

#include "entt/entity/fwd.hpp"

namespace ShapeGame
{
    class ISystem
    {
    public:
        virtual ~ISystem() = default;
        virtual void Initialize() {}
        virtual void Update(float DeltaTime) = 0;
    };

    template <class T> class SystemBase : public ISystem
    {
    protected:
        entt::registry& Registry() {}
    };
} // namespace ShapeGame