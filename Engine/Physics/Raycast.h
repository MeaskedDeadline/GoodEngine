#pragma once
#include "../Core/Core.h"
#include "Ray.h"

namespace Good
{

    class Raycast
    {
    public:
        static bool Cast(const Ray& ray, float32 maxDist, RayHit& hit);
        static std::vector<RayHit> CastAll(const Ray& ray, float32 maxDist);
        static bool Spherecast(const Vec3& origin, float32 radius,
                                const Vec3& direction, float32 maxDist,
                                RayHit& hit);

    private:
        static bool SpherecastImpl(const Vec3& origin, float32 radius,
                                    const Vec3& direction, float32 maxDist,
                                    RayHit& hit);
    };

} // namespace Good