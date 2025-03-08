
#pragma once

#include "BaseComponents.h"
#include "SystemBase.h"
#include "raylib.h"
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace ShapeGame
{
    class IRenderStrategy
    {
    public:
        virtual ~IRenderStrategy() = default;

        virtual void Execute() = 0;
    };

    class RenderSystem : public SystemBase<RenderSystem>
    {
    public:
        RenderSystem();

        virtual void Update(float DeltaTime) override;

    private:
        std::unordered_map<std::type_index, std::unique_ptr<IRenderStrategy>> strategies;
        Camera2D camera;
    };
} // namespace ShapeGame