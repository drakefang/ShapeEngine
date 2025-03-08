

#pragma once

#include "BaseComponents.h"
#include "Renderer/RenderSystem.h"

namespace ShapeGame
{
    class LineRenderer : public IRenderStrategy
    {
    public:
        virtual void Execute() override;
    };
} // namespace ShapeGame