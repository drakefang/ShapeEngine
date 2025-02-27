
#include "System.h"

namespace ShapeGame
{
    class TransformSystem : public System
    {
    public:
        using System::System;

        virtual void Update(float DeltaTime) override;
    };
} // namespace ShapeGame