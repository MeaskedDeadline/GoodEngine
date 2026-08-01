#include "SphereCollider.h"
#include "../Math/MathCommon.h"

namespace Good
{

    SphereCollider::SphereCollider() = default;
    SphereCollider::SphereCollider(float32 r) : m_Radius(r) {}

    AABB SphereCollider::GetAABB(const Transform& t) const
    {
        float32 maxScale = Math::Max(t.scale.x, Math::Max(t.scale.y, t.scale.z));
        float32 r = m_Radius * maxScale;
        Vec3 center = t.position + m_Offset;
        return AABB(center - Vec3(r,r,r), center + Vec3(r,r,r));
    }

    bool SphereCollider::Raycast(const Ray& ray, const Transform& t, RayHit& hit) const
    {
        float32 maxScale = Math::Max(t.scale.x, Math::Max(t.scale.y, t.scale.z));
        Vec3 center = t.position + m_Offset;
        return ray.IntersectsSphere(center, m_Radius * maxScale, hit);
    }

} // namespace Good