
#include "Object.h"

#include "MetaRegistry.h"

namespace ShapeGame
{
    void Object::Serialize(Archive &archive)
    {
    }

    entt::id_type Object::GetTypeId() const
    {
        return entt::resolve<Object>().id();
    }

    void Object::RegisterMeta()
    {
        using namespace entt::literals;
        entt::meta<Object>()
            .type("Object"_hs)
            .ctor<>()
            .data<&Object::ObjectId, entt::as_ref_t>("ObjectId"_hs)
        ;
    }
}

static ShapeGame::AutoRegistrar RegistrarObject(&ShapeGame::Object::RegisterMeta);
