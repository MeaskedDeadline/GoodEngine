#pragma once
#include "../Core/Core.h"

namespace Good
{

    struct PhysicsMaterial
    {
        float32 friction{0.5f};
        float32 bounciness{0.0f};
        float32 density{1.0f};

        PhysicsMaterial() = default;
        PhysicsMaterial(float32 friction, float32 bounce, float32 density);

        static PhysicsMaterial Default();
        static PhysicsMaterial Ice();
        static PhysicsMaterial Rubber();
        static PhysicsMaterial Metal();
        static PhysicsMaterial Wood();
    };

} // namespace Good