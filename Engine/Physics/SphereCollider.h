#pragma once
#include "Collider.h"

namespace Good
{

    class SphereCollider final : public Collider
    {
    public:
        SphereCollider();
        explicit SphereCollider(float32 radius);

        ColliderShape GetShape() const override { return ColliderShape::Sphere; }
        AABB  GetAABB(const Transform& t) const override;
        bool  Raycast(const Ray& ray, const Transform& t, RayHit& hit) const override;

        void    SetRadius(float32 r) { m_Radius = r; }
        float32 GetRadius()    const { return m_Radius; }

    private:
        float32 m_Radius{0.5f};
    };

} // namespace Good