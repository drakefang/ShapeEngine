//
// Created by yuyzc on 2025/6/29.
//

#pragma once

#include "Core/Object.h"
#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include <vector>
#include <memory>
#include <string>

namespace ShapeGame
{
    class ActorComponent;
    class World;

    class Actor : public Object, public std::enable_shared_from_this<Actor>
    {
    public:
        Actor() = default;

        ~Actor() override = default;

        static void RegisterMeta();

        void SetLocalPosition(const glm::vec3& position);
        glm::vec3 GetLocalPosition() const;
        void SetLocalRotation(const glm::quat& rotation);

        glm::quat GetLocalRotation() const;

        void SetLocalScale(const glm::vec3& scale);
        glm::vec3 GetLocalScale() const;

        void SetWorldPosition(const glm::vec3& position);
        glm::vec3 GetWorldPosition() const;
        glm::vec3 GetWorldScale() const;
        void SetWorldRotation(const glm::quat& rotation);
        glm::quat GetWorldRotation() const;
        void SetWorldTransform(const glm::mat4& transform);
        glm::mat4x4 GetWorldTransformMatrix() const;

        void SetParent(const std::shared_ptr<Actor>& newParent);

        std::shared_ptr<Actor> GetParent() const;

        std::vector<std::shared_ptr<Actor> > GetChildren() const;

        template <typename T, typename... Args>
        std::shared_ptr<T> AddComponent(Args&&... args);

        template <typename T>
        std::shared_ptr<T> GetComponent() const;

        template <typename T>
        std::vector<std::shared_ptr<T> > GetComponents() const;

        void Initialize();

        void Start();

        void Update(float deltaTime);

        void OnDestroy();

        auto GetWorld() const;

        bool IsActive() const
        {
            return bIsActive;
        }

        void SetActive(bool active);

    private:
        void AddChild(const std::shared_ptr<Actor>& child);

        void RemoveChild(const std::shared_ptr<Actor>& child);

        void UpdateWorldTransform() const;

    private:
        friend class World;
        std::weak_ptr<World> OwningWorld;
        bool bIsActive = true;
        bool bHasStarted = false;

        std::vector<std::shared_ptr<ActorComponent> > Components;

        glm::vec3 LocalPosition = {0.0f, 0.0f, 0.0f};
        glm::quat LocalRotation = {0.0f, 0.0f, 0.0f, 1.0f};
        glm::vec3 LocalScale = {1.0f, 1.0f, 1.0f};

        mutable glm::mat4x4 WorldTransform = glm::mat4(1.f);
        mutable bool bIsWorldTransformDirty = true;

        std::weak_ptr<Actor> Parent;
        std::vector<std::shared_ptr<Actor> > Children;
    };

    template <typename T, typename... Args>
    std::shared_ptr<T> Actor::AddComponent(Args&&... args)
    {
        static_assert(std::is_base_of<ActorComponent, T>::value, "T must be a derivative of Component.");

        auto newComponent = std::make_shared<T>(std::forward<Args>(args)...);
        newComponent->owner = this;
        Components.push_back(newComponent);

        if (OwningWorld)
        {
            newComponent->Initialize();
        }
        if (bHasStarted)
        {
            newComponent->Start();
        }

        return newComponent;
    }

    template <typename T>
    std::shared_ptr<T> Actor::GetComponent() const
    {
        for (const auto& comp : Components)
        {
            if (auto castedComp = std::dynamic_pointer_cast<T>(comp))
            {
                return castedComp;
            }
        }
        return nullptr;
    }

    template <typename T>
    std::vector<std::shared_ptr<T> > Actor::GetComponents() const
    {
        std::vector<std::shared_ptr<T> > result;
        for (const auto& comp : Components)
        {
            if (auto castedComp = std::dynamic_pointer_cast<T>(comp))
            {
                result.push_back(castedComp);
            }
        }
        return result;
    }
} // ShapeGame