

#include "TransformSystem.h"
#include "Components/Transform.h"
#include "raymath.h"

namespace ShapeGame
{
    void TransformSystem::Update(float DeltaTime)
    {
        auto view = registry.view<TransformComponent, VelocityComponent, RotatorComponent>();
        view.each(
            [this, DeltaTime](const auto& entity, TransformComponent& rc, VelocityComponent& vc, RotatorComponent& rsc)
            {
                vc.velocity = Vector2Add(vc.velocity, Vector2Scale(vc.accelerate, DeltaTime));
                rc.pos = Vector2Add(rc.pos, Vector2Scale(vc.velocity, DeltaTime));

                rc.rot = Lerp(rc.rot, rsc.target, rsc.speed * DeltaTime);
                if (rc.rot >= 360.f)
                {
                    rc.rot -= 360.f;
                }
                else if (rc.rot <= -360.f)
                {
                    rc.rot += 360.f;
                }
            });
    }
} // namespace ShapeGame