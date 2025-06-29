//
// Created by yuyzc on 2025/6/29.
//

#include "World.h"

#include "Actor.h"
#include "Core/MetaRegistry.h"

namespace ShapeGame
{
    void World::RegisterMeta()
    {
        using namespace entt::literals;
        entt::meta_factory<World>()
            .type("World"_hs)
            //.ctor<std::shared_ptr<World>(*)()>(entt::invoker_v<&std::make_shared<World>>)
            .base<Object>();
    }

    static AutoRegistrar registrar_World(&World::RegisterMeta);

    World::World()
    {
        SceneRoot = std::make_shared<Actor>();
        SceneRoot->OwningWorld = shared_from_this();
        SceneRoot->ObjectId = IdGenerator.Generate();
        AllActors.push_back(SceneRoot);
    }

    void World::DestroyActor(const std::shared_ptr<Actor>& actorToDestroy)
    {
        if (!actorToDestroy) return;

        actorToDestroy->OnDestroy();
        actorToDestroy->SetParent(nullptr);

        auto childrenCopy = actorToDestroy->GetChildren();
        for (const auto& child : childrenCopy)
        {
            DestroyActor(child);
        }

        std::lock_guard<std::mutex> lock(WorldMutex);
        std::erase_if(AllActors, [&](const auto& p){ return p == actorToDestroy; });
        std::erase_if(PendingActors, [&](const auto& p){ return p == actorToDestroy; });
    }

    void World::Tick(float deltaTime)
    {
        StartNewActors();

        for (const auto& actor : AllActors)
        {
            if (actor->IsActive())
            {
                actor->Update(deltaTime);
            }
        }
    }

    Actor* World::GetSceneRoot() const
    {
        return SceneRoot.get();
    }

    void World::StartNewActors()
    {
        if (PendingActors.empty()) return;

        auto actorsToStart = PendingActors;
        PendingActors.clear();

        for (const auto& actor : actorsToStart)
        {
            if (actor->IsActive())
            {
                actor->Start();
            }
        }
    }
} // ShapeGame