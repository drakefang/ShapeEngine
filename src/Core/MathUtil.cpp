

#include "MathUtil.h"


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
} // namespace ShapeGame