#pragma once
#include "../Core/Core.h"
#include "../Math/Vec3.h"

namespace Good
{

    struct AABB
    {
        Vec3 min{0,0,0};
        Vec3 max{0,0,0};

        AABB() = default;
        AABB(const Vec3& min, const Vec3& max);

        Vec3    Center()     const;
        Vec3    Size()       const;
        Vec3    HalfSize()   const;

        bool    Contains(const Vec3& point) const;
        bool    Intersects(const AABB& other) const;
        AABB    Expanded(float32 amount) const;
        AABB    Merged(const AABB& other) const;
        float32 Volume() const;

        static AABB FromCenterSize(const Vec3& center, const Vec3& size);
    };

} // namespace Good