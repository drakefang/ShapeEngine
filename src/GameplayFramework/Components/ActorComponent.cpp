//
// Created by yuyzc on 2025/6/29.
//

#include "ActorComponent.h"

#include <entt/meta/factory.hpp>

#include "Core/MetaRegistry.h"

namespace ShapeGame
{
    auto ActorComponent::GetOwner() const
    {
        return Owner.lock();
    }

    void ActorComponent::RegisterMeta()
    {
        using namespace entt::literals;
        entt::meta_factory<ActorComponent>()
            .type("Component"_hs)
            //.ctor<std::shared_ptr<Component>(*)()>(entt::invoker_v<&std::make_shared<Component>>)
            .base<Object>();
    }

    static AutoRegistrar registrar_Component(&ActorComponent::RegisterMeta);
} // ShapeGame