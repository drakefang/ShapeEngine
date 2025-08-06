//
// Created by yuyzc on 2025/8/4.
//


#pragma once
#include <entt/entt.hpp>
#include "Renderer/Renderer2D.h"
#include "Components/TransformComponent.h"
#include "Components/RenderComponent.h"

namespace ShapeGame
{
    namespace RenderSystem
    {
        inline void Update(entt::registry& registry, Renderer2D& renderer)
        {
            auto view = registry.view<const TransformComponent, const RenderComponent>();

            for (auto entity : view)
            {
                const auto& transform = view.get<const TransformComponent>(entity);
                const auto& renderable = view.get<const RenderComponent>(entity);

                for (const auto& geometry : renderable.geometries)
                {
                    renderer.DrawShape(geometry, transform.GetTransform());
                }
            }
        }
    }
}