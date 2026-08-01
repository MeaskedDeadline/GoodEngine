#pragma once
#include "../Core/Core.h"
#include "../Math/Vec3.h"

namespace Good
{

    class MathUtils
    {
    public:
        static float32 Map(float32 value, float32 inMin, float32 inMax,
                            float32 outMin, float32 outMax);
        static float32 Wrap(float32 value, float32 min, float32 max);
        static float32 PingPong(float32 t, float32 length);
        static float32 SmoothDamp(float32 current, float32 target,
                                   float32& velocity, float32 smoothTime, float32 dt);
        static Vec3 SmoothDamp(const Vec3& current, const Vec3& target,
                                Vec3& velocity, float32 smoothTime, float32 dt);
        static float32 MoveTowards(float32 current, float32 target, float32 maxDelta);
        static Vec3    MoveTowards(const Vec3& current, const Vec3& target, float32 maxDelta);
    };

} // namespace Good