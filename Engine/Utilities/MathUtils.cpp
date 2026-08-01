#include "MathUtils.h"
#include "../Math/MathCommon.h"

namespace Good
{

    float32 MathUtils::Map(float32 v, float32 inMin, float32 inMax,
                            float32 outMin, float32 outMax)
    {
        if (Math::Approximately(inMin, inMax)) return outMin;
        return outMin + (v - inMin) / (inMax - inMin) * (outMax - outMin);
    }

    float32 MathUtils::Wrap(float32 v, float32 mn, float32 mx)
    {
        float32 range = mx - mn;
        if (range <= 0.0f) return mn;
        return v - range * Math::Floor((v - mn) / range);
    }

    float32 MathUtils::PingPong(float32 t, float32 length)
    {
        if (length <= 0.0f) return 0.0f;
        float32 wrapped = Wrap(t, 0.0f, length * 2.0f);
        return length - Math::Abs(wrapped - length);
    }

    float32 MathUtils::SmoothDamp(float32 cur, float32 tgt,
                                   float32& vel, float32 smoothTime, float32 dt)
    {
        smoothTime = Math::Max(0.0001f, smoothTime);
        float32 omega = 2.0f / smoothTime;
        float32 x     = omega * dt;
        float32 exp   = 1.0f / (1.0f + x + 0.48f*x*x + 0.235f*x*x*x);
        float32 delta = cur - tgt;
        float32 temp  = (vel + omega * delta) * dt;
        vel           = (vel - omega * temp) * exp;
        return tgt + (delta + temp) * exp;
    }

    Vec3 MathUtils::SmoothDamp(const Vec3& cur, const Vec3& tgt,
                                Vec3& vel, float32 st, float32 dt)
    {
        return Vec3(
            SmoothDamp(cur.x, tgt.x, vel.x, st, dt),
            SmoothDamp(cur.y, tgt.y, vel.y, st, dt),
            SmoothDamp(cur.z, tgt.z, vel.z, st, dt));
    }

    float32 MathUtils::MoveTowards(float32 cur, float32 tgt, float32 maxD)
    {
        if (Math::Abs(tgt - cur) <= maxD) return tgt;
        return cur + Math::Sign(static_cast<float32>(tgt - cur)) * maxD;
    }

    Vec3 MathUtils::MoveTowards(const Vec3& cur, const Vec3& tgt, float32 maxD)
    {
        Vec3 delta = tgt - cur;
        float32 dist = delta.Length();
        if (dist <= maxD || dist < Math::EPSILON) return tgt;
        return cur + delta / dist * maxD;
    }

} // namespace Good