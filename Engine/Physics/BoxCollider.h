#pragma once
#include "Collider.h"

namespace Good
{

    class BoxCollider final : public Collider
    {
    public:
        BoxCollider();
        explicit BoxCollider(const Vec3& size);

        ColliderShape GetShape() const override { return ColliderShape::Box; }
        AABB  GetAABB(const Transform& t) const override;
        bool  Raycast(const Ray& ray, const Transform& t, RayHit& hit) const override;

        void        SetSize(const Vec3& s) { m_Size = s; }
        const Vec3& GetSize()        const { return m_Size; }

    private:
        Vec3 m_Size{1,1,1};
    };

} // namespace Good