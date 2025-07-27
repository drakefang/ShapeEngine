//
// Created by yuyzc on 2025/6/29.
//

#pragma once
#include "Core/Object.h"

namespace ShapeGame
{
    class ActorComponent : public Object
    {
    public:
        ActorComponent() = default;
        static void RegisterMeta();

        virtual void Initialize() {}
        virtual void Start() {}
        virtual void Update(float deltaTime) {}
        virtual void OnDestroy() {}

        auto GetOwner() const;

    private:
        friend class Actor;
        std::weak_ptr<Actor> Owner;
    };
} // ShapeGame
