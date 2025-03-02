
#pragma once

#include "System.h"
#include "raylib.h"

namespace ShapeGame
{
    class RenderSystem_ : public System
    {
    public:
        RenderSystem_(entt::registry& reg);
        virtual void Update(float DeltaTime) override;

    protected:
        void GetVertices(
            std::vector<Vector2>& vertices,
            const struct SegmentComponent& segment,
            float thickness,
            const Vector2& loc,
            float rot,
            bool hasThickness,
            bool hasEndcap);

    private:
        Camera2D camera;
    };
} // namespace ShapeGame