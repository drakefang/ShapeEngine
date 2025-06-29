//
// Created by yuyzc on 2025/6/29.
//

#include "Actor.h"

#include "Core/MetaRegistry.h"
#include "Components/ActorComponent.h"

#include "glm/gtx/matrix_decompose.hpp"

namespace ShapeGame
{
    void Actor::RegisterMeta()
    {
        using namespace entt::literals;
        entt::meta_factory<Actor>()
            .type("Actor"_hs)
            //.ctor<std::shared_ptr<Actor>(*)()>(entt::invoker_v<&std::make_shared<Actor>>)
            .base<Object>();
    }

    static AutoRegistrar registrar_Actor(&Actor::RegisterMeta);

    void Actor::Initialize()
    {
        for (const auto& comp : Components)
        {
            comp->Initialize();
        }
    }

    void Actor::Start()
    {
        if (bHasStarted)
            return;

        for (const auto& comp : Components)
        {
            comp->Start();
        }
        bHasStarted = true;
    }

    void Actor::Update(float deltaTime)
    {
        for (const auto& comp : Components)
        {
            comp->Update(deltaTime);
        }
    }

    void Actor::OnDestroy()
    {
        for (const auto& comp : Components)
        {
            comp->OnDestroy();
        }
    }

    auto Actor::GetWorld() const
    {
        return OwningWorld.lock();
    }

    void Actor::SetActive(bool active)
    {
        bIsActive = active;
    }

    void Actor::SetLocalPosition(const glm::vec3& position)
    {
        LocalPosition = position;
        bIsWorldTransformDirty = true;
    }

    glm::vec3 Actor::GetLocalPosition() const
    {
        return LocalPosition;
    }

    void Actor::SetLocalRotation(const glm::quat& rotation)
    {
        LocalRotation = rotation;
        bIsWorldTransformDirty = true;
    }

    glm::quat Actor::GetLocalRotation() const
    {
        return LocalRotation;
    }

    void Actor::SetLocalScale(const glm::vec3& scale)
    {
        LocalScale = scale;
        bIsWorldTransformDirty = true;
    }

    glm::vec3 Actor::GetLocalScale() const
    {
        return LocalScale;
    }

    void Actor::SetWorldPosition(const glm::vec3& position)
    {
        glm::mat4 matrix = GetWorldTransformMatrix();
        matrix[3] = glm::vec4(position, 1.0f);
        SetWorldTransform(matrix);
    }

    glm::vec3 Actor::GetWorldPosition() const
    {
        const glm::mat4x4 worldMatrix = GetWorldTransformMatrix();
        return glm::vec3(worldMatrix[3]);
    }

    glm::vec3 Actor::GetWorldScale() const
    {
        glm::vec3 scale, translation, skew;
        glm::vec4 perspective;
        glm::quat rotation;
        glm::decompose(GetWorldTransformMatrix(), scale, rotation, translation, skew, perspective);
        return scale;
    }

    void Actor::SetWorldRotation(const glm::quat& rotation)
    {
        glm::vec3 currentWorldPosition = GetWorldPosition();
        glm::vec3 currentWorldScale = GetWorldScale();

        glm::mat4 newWorldTransform = glm::mat4(1.0f);
        newWorldTransform = glm::translate(newWorldTransform, currentWorldPosition);
        newWorldTransform = newWorldTransform * glm::mat4_cast(rotation);
        newWorldTransform = glm::scale(newWorldTransform, currentWorldScale);

        SetWorldTransform(newWorldTransform);

    }

    glm::quat Actor::GetWorldRotation() const
    {
        // 如果有非单位缩放，glm::quat_cast获取的结果可能出问题，因此不使用quat_cast
        glm::vec3 scale, translation, skew;
        glm::vec4 perspective;
        glm::quat rotation;
        glm::decompose(GetWorldTransformMatrix(), scale, rotation, translation, skew, perspective);
        return rotation;
    }

    void Actor::SetWorldTransform(const glm::mat4& transform)
    {
        glm::mat4 localTransform;

        // 获取父节点的世界变换矩阵的逆矩阵
        if (auto parentPtr = Parent.lock())
        {
            glm::mat4 parentInverseWorldTransform = glm::inverse(parentPtr->GetWorldTransformMatrix());
            localTransform = parentInverseWorldTransform * transform;
        }
        else
        {
            localTransform = transform;
        }

        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(localTransform, LocalScale, LocalRotation, LocalPosition, skew, perspective);

        bIsWorldTransformDirty = true;
    }

    glm::mat4x4 Actor::GetWorldTransformMatrix() const
    {
        UpdateWorldTransform();
        return WorldTransform;
    }

    void Actor::UpdateWorldTransform() const
    {
        if (bIsWorldTransformDirty)
        {
            glm::mat4x4 localTransform(1.f);
            localTransform = glm::translate(localTransform, LocalPosition);
            localTransform = localTransform * glm::mat4_cast(LocalRotation);
            localTransform = glm::scale(localTransform, LocalScale);

            if (auto parentPtr = Parent.lock())
            {
                WorldTransform = parentPtr->GetWorldTransformMatrix() * localTransform;
            }
            else
            {
                WorldTransform = localTransform;
            }

            for (const auto& child : Children)
            {
                child->bIsWorldTransformDirty = true;
            }

            bIsWorldTransformDirty = false;
        }
    }

    void Actor::SetParent(const std::shared_ptr<Actor>& newParent)
    {
        if (auto oldParent = Parent.lock())
        {
            oldParent->RemoveChild(std::static_pointer_cast<Actor>(shared_from_this()));
        }

        Parent = newParent;

        if (newParent)
        {
            newParent->AddChild(std::static_pointer_cast<Actor>(shared_from_this()));
        }

        bIsWorldTransformDirty = true;
    }

    std::shared_ptr<Actor> Actor::GetParent() const
    {
        return Parent.lock();
    }

    std::vector<std::shared_ptr<Actor> > Actor::GetChildren() const
    {
        return Children;
    }

    void Actor::AddChild(const std::shared_ptr<Actor>& child)
    {
        for (const auto& c : Children)
        {
            if (c == child)
                return;
        }
        Children.push_back(child);
    }

    void Actor::RemoveChild(const std::shared_ptr<Actor>& child)
    {
        for (auto it = Children.begin(); it != Children.end(); ++it)
        {
            if (*it == child)
            {
                Children.erase(it);
                return;
            }
        }
    }
} // ShapeGame