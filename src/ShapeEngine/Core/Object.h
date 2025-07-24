
#pragma once

#include <string>
#include <cstdint>
#include <entt/entt.hpp>

#include "Core/PlatformDefine.h"

namespace ShapeEngine
{
    class WriterArchive;
    class ReaderArchive;

    class ObjectIdGenerator
    {
    public:
        ObjectIdGenerator() = default;

        uint64_t Generate()
        {
            return nextId.fetch_add(1, std::memory_order_relaxed);
        }

    private:
        std::atomic<uint64_t> nextId{1};
    };

    class SHAPE_ENGINE_API Object
    {
    public:
        Object() = default;
        virtual ~Object() = default;

        virtual void Save(WriterArchive& writer) const;
        virtual void Load(ReaderArchive& reader);

        [[nodiscard]] entt::id_type GetTypeId() const;

        static void RegisterMeta();

    public:
        uint64_t ObjectId = 0;
    };
}