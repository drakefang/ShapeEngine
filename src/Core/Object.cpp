
#include "Object.h"
#include "Archive.h"
#include "MetaRegistry.h"
#include "spdlog/spdlog.h"

#include <chrono>

#include "Logger.h"

namespace
{
    void SaveAny(ShapeGame::WriterArchive& archive, entt::meta_any& any)
    {
        if (auto* value = any.try_cast<int32_t>())
        {
            archive << *value;
        }
        else if (auto* value = any.try_cast<uint64_t>())
        {
            archive << *value;
        }
        else if (auto* value = any.try_cast<float>())
        {
            archive << *value;
        }
        else if (auto* value = any.try_cast<std::string>())
        {
            archive << *value;
        }
        else if (auto* value = any.try_cast<std::shared_ptr<ShapeGame::Object>>())
        {
            archive << *value;
        }
        else
        {
            ShapeGame::Logger()->error("Type: {0} is not handled by the load archive!", any.type().info().name());
        }
    }

    void LoadAny(ShapeGame::ReaderArchive& archive, entt::meta_any& any)
    {
        if (auto* value = any.try_cast<int32_t>())
        {
            archive >> *value;
        }
        else if (auto* value = any.try_cast<uint64_t>())
        {
            archive >> *value;
        }
        else if (auto* value = any.try_cast<float>())
        {
            archive >> *value;
        }
        else if (auto* value = any.try_cast<std::string>())
        {
            archive >> *value;
        }
        else if (auto* value = any.try_cast<std::shared_ptr<ShapeGame::Object>>())
        {
            archive >> *value;
        }
        else
        {
            ShapeGame::Logger()->error("Type: {0} is not handled by the load archive!", any.type().info().name());
        }
    }
}

namespace ShapeGame
{
    void Object::Save(WriterArchive &archive) const
    {
        auto typeId = entt::type_id(*this);
        if (auto meta = entt::resolve(typeId))
        {
            for (auto metadata : meta.data())
            {
                auto member = metadata.second.get(*this);
                SaveAny(archive, member);
            }
        }
    }

    void Object::Load(ReaderArchive &archive)
    {
        auto typeId = entt::type_id(*this);
        if (auto meta = entt::resolve(typeId))
        {
            for (auto metadata : meta.data())
            {
                auto member = metadata.second.get(*this);
                LoadAny(archive, member);
            }
        }
    }

    entt::id_type Object::GetTypeId() const
    {
        return entt::resolve<Object>().id();
    }

    void Object::RegisterMeta()
    {
        using namespace entt::literals;
        entt::meta_factory<Object>()
            .type("Object"_hs)
            .ctor<>()
            .data<&Object::ObjectId, entt::as_ref_t>("ObjectId"_hs)
        ;
    }
}

static ShapeGame::AutoRegistrar RegistrarObject(&ShapeGame::Object::RegisterMeta);
