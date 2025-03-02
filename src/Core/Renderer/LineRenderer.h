

#pragma once

#include "Core/BaseComponents.h"
#include "Core/Renderer/RenderSystem.h"

namespace ShapeGame
{
    class LineRenderer : public IRenderStrategy
    {
    public:
        virtual void Execute() override;
    };
} // namespace ShapeGame