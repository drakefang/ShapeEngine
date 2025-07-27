//
// Created by yuyzc on 2025/6/29.
//

#pragma once
#include <memory>
#include <mutex>

#include "Core/Object.h"

namespace ShapeGame
{
    class Actor;
    class World : public Object, public std::enable_shared_from_this<World>
    {
    public:
        World();
        ~World() override = default;
        static void RegisterMeta();

        template<typename T, typename... Args>
        std::shared_ptr<T> SpawnActor(Args&&... args);

        void DestroyActor(const std::shared_ptr<Actor>& actorToDestroy);

        void Tick(float deltaTime);

        Actor* GetSceneRoot() const;

    private:
        void StartNewActors();

    private:
        ObjectIdGenerator IdGenerator;
        std::vector<std::shared_ptr<Actor>> AllActors;
        std::vector<std::shared_ptr<Actor>> PendingActors;
        std::shared_ptr<Actor> SceneRoot;
        mutable std::mutex WorldMutex;
    };

    template<typename T, typename... Args>
    std::shared_ptr<T> World::SpawnActor(Args&&... args)
    {
        static_assert(std::is_base_of<Actor, T>::value, "T must be a derivative of Actor.");

        auto newActor = std::make_shared<T>(std::forward<Args>(args)...);
        {
            std::lock_guard<std::mutex> lock(WorldMutex);
            AllActors.push_back(newActor);
        }

        newActor->world = this;
        newActor->objectId = IdGenerator.Generate();

        newActor->SetParent(SceneRoot);

        newActor->Initialize();
        {
            std::lock_guard<std::mutex> lock(WorldMutex);
            PendingActors.push_back(newActor);
        }

        return newActor;
    }
} // ShapeGame
