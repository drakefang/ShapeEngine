
#pragma once

#include <string>
#include <cstdint>
#include <entt/entt.hpp>

namespace ShapeGame
{
    class WriterArchive;
    class ReaderArchive;

    class Object
    {
    public:
        Object() = default;
        virtual ~Object() = default;

        virtual void Save(WriterArchive& writer) const;
        virtual void Load(ReaderArchive& reader);

        entt::id_type GetTypeId() const;

        static void RegisterMeta();

    public:
        uint64_t ObjectId = 0;
    };
}