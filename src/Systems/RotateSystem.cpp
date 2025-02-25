

#include "RotateSystem.h"
#include "Components/Transform.h"


namespace ShapeGame
{
    void RotateSystem::Update(float DeltaTime)
    {
        auto view = registry.view<RotationComponent, RotSpeedComponent>();
        view.each([this, DeltaTime](const auto& entity, RotationComponent& rc, RotSpeedComponent& rsc)
        {
            rc.rot += rsc.speed * DeltaTime;
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
}