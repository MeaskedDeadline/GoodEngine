#pragma once
#include "../Core/Core.h"
#include "../Math/Vec3.h"
#include "../Math/Color.h"

namespace Good
{

    struct Particle
    {
        Vec3    position{0,0,0};
        Vec3    velocity{0,0,0};
        Vec3    acceleration{0,0,0};
        Color   color{1,1,1,1};
        Color   startColor{1,1,1,1};
        Color   endColor{1,1,1,0};
        float32 size{1.0f};
        float32 startSize{1.0f};
        float32 endSize{0.0f};
        float32 lifetime{1.0f};
        float32 age{0.0f};
        float32 rotation{0.0f};
        float32 angularVelocity{0.0f};
        bool    alive{false};

        float32 GetLifeRatio() const
        {
            return lifetime > 0.0f ? age / lifetime : 1.0f;
        }
    };

} // namespace Good