
#pragma once

#include "Core/ShapeComponents.h"
#include "RenderSystem.h"

namespace ShapeGame
{
    class PolygonRenderer : public IRenderStrategy
    {
    public:
        virtual void Execute() override;
    };
} // namespace ShapeGame