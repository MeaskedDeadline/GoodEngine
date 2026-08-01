#pragma once
#include "../Core/Core.h"
#include "../Math/Vec3.h"

namespace Good
{

    struct RayHit
    {
        Vec3    point{0,0,0};
        Vec3    normal{0,1,0};
        float32 distance{0.0f};
        bool    hit{false};
    };

    class Ray
    {
    public:
        Ray() = default;
        Ray(const Vec3& origin, const Vec3& direction);

        Vec3 GetPoint(float32 t) const;

        bool IntersectsAABB(const struct AABB& aabb, RayHit& hit) const;
        bool IntersectsSphere(const Vec3& center, float32 radius, RayHit& hit) const;
        bool IntersectsPlane(const Vec3& planeNormal, float32 planeD, RayHit& hit) const;
        bool IntersectsTriangle(const Vec3& a, const Vec3& b, const Vec3& c, RayHit& hit) const;

        Vec3 origin{0,0,0};
        Vec3 direction{0,0,-1};
    };

} // namespace Good