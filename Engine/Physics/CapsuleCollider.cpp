#include "CapsuleCollider.h"

namespace Good
{

    CapsuleCollider::CapsuleCollider() = default;
    CapsuleCollider::CapsuleCollider(float32 r, float32 h) : m_Radius(r), m_Height(h) {}

    AABB CapsuleCollider::GetAABB(const Transform& t) const
    {
        Vec3 center = t.position + m_Offset;
        float32 r = m_Radius * Math::Max(t.scale.x, t.scale.z);
        float32 h = m_Height * t.scale.y * 0.5f;
        return AABB(center - Vec3(r, h+r, r), center + Vec3(r, h+r, r));
    }

    bool CapsuleCollider::Raycast(const Ray& ray, const Transform& t, RayHit& hit) const
    {
        // Упрощённо: считаем как сферу
        Vec3 center = t.position + m_Offset;
        return ray.IntersectsSphere(center, m_Radius, hit);
    }

} // namespace Good