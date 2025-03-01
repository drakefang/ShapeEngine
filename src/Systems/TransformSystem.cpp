

#include "TransformSystem.h"
#include "Components/Transform.h"
#include "raymath.h"
#include <cfloat>
#include <cmath>

namespace ShapeGame
{
    float AngleDifference(float Current, float Target)
    {
        float Diff = fmodf(Target - Current, 360.f);
        Diff = fmodf(Diff + 180.f, 360.f) - 180.f;
        return Diff;
    }

    float AngleLerp(float Current, float Target, float Speed, float DeltaTime)
    {
        float Remaining = AngleDifference(Current, Target);

        if (fabsf(Remaining) <= FLT_EPSILON)
        {
            return Target;
        }

        float Step = copysignf(fminf(fabsf(Remaining), Speed * DeltaTime), Remaining);
        return fmodf(Current + Step + 360.f, 360.f);
    }

    void TransformSystem::Update(float DeltaTime)
    {
        auto view = registry.view<TransformComponent, VelocityComponent>();
        view.each(
            [this, DeltaTime](const auto& entity, TransformComponent& rc, VelocityComponent& vc)
            {
                vc.velocity = Vector2Add(vc.velocity, Vector2Scale(vc.accelerate, DeltaTime));
                rc.pos = Vector2Add(rc.pos, Vector2Scale(vc.velocity, DeltaTime));
            });

        auto view1 = registry.view<TransformComponent, RotatorComponent>();
        view1.each(
            [DeltaTime](const auto& entity, TransformComponent& rc, RotatorComponent& rsc)
            {
                rc.rot = AngleLerp(rc.rot, rsc.target, rsc.speed, DeltaTime);
            });
    }
} // namespace ShapeGame