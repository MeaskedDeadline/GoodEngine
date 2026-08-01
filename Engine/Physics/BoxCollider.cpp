#include "BoxCollider.h"

namespace Good
{

    BoxCollider::BoxCollider() = default;
    BoxCollider::BoxCollider(const Vec3& size) : m_Size(size) {}

    AABB BoxCollider::GetAABB(const Transform& t) const
    {
        Vec3 halfSize = (m_Size * t.scale) * 0.5f;
        Vec3 center   = t.position + m_Offset;
        return AABB(center - halfSize, center + halfSize);
    }

    bool BoxCollider::Raycast(const Ray& ray, const Transform& t, RayHit& hit) const
    {
        AABB aabb = GetAABB(t);
        return ray.IntersectsAABB(aabb, hit);
    }

} // namespace Good