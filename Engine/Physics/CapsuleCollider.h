#pragma once
#include "Collider.h"

namespace Good
{

    class CapsuleCollider final : public Collider
    {
    public:
        CapsuleCollider();
        CapsuleCollider(float32 radius, float32 height);

        ColliderShape GetShape() const override { return ColliderShape::Capsule; }
        AABB  GetAABB(const Transform& t) const override;
        bool  Raycast(const Ray& ray, const Transform& t, RayHit& hit) const override;

        void    SetRadius(float32 r) { m_Radius = r; }
        void    SetHeight(float32 h) { m_Height = h; }
        float32 GetRadius()    const { return m_Radius; }
        float32 GetHeight()    const { return m_Height; }

    private:
        float32 m_Radius{0.5f};
        float32 m_Height{2.0f};
    };

} // namespace Good