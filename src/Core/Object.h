
#pragma once

#include <string>
#include <cstdint>
#include <entt/entt.hpp>

namespace ShapeGame
{
    class Archive;

    class Object
    {
    public:
        virtual ~Object() = default;

        void Serialize(Archive& archive);

        entt::id_type GetTypeId() const;

        static void RegisterMeta();

    public:
        uint64_t ObjectId = 0;
    };
}