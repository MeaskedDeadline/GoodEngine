#include "AABB.h"
#include "../Math/MathCommon.h"

namespace Good
{

    AABB::AABB(const Vec3& mn, const Vec3& mx) : min(mn), max(mx) {}

    Vec3 AABB::Center()   const { return (min + max) * 0.5f; }
    Vec3 AABB::Size()     const { return max - min; }
    Vec3 AABB::HalfSize() const { return (max - min) * 0.5f; }

    bool AABB::Contains(const Vec3& p) const
    {
        return p.x >= min.x && p.x <= max.x
            && p.y >= min.y && p.y <= max.y
            && p.z >= min.z && p.z <= max.z;
    }

    bool AABB::Intersects(const AABB& o) const
    {
        return min.x <= o.max.x && max.x >= o.min.x
            && min.y <= o.max.y && max.y >= o.min.y
            && min.z <= o.max.z && max.z >= o.min.z;
    }

    AABB AABB::Expanded(float32 a) const
    {
        Vec3 expand(a,a,a);
        return {min - expand, max + expand};
    }

    AABB AABB::Merged(const AABB& o) const
    {
        return {
            Vec3(Math::Min(min.x, o.min.x), Math::Min(min.y, o.min.y), Math::Min(min.z, o.min.z)),
            Vec3(Math::Max(max.x, o.max.x), Math::Max(max.y, o.max.y), Math::Max(max.z, o.max.z))
        };
    }

    float32 AABB::Volume() const
    {
        Vec3 s = Size();
        return s.x * s.y * s.z;
    }

    AABB AABB::FromCenterSize(const Vec3& c, const Vec3& s)
    {
        Vec3 half = s * 0.5f;
        return {c - half, c + half};
    }

} // namespace Good