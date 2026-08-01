#pragma once
#include "../Core/Core.h"
#include "../Math/Vec3.h"
#include "../Math/Transform.h"
#include "AABB.h"
#include "Ray.h"

namespace Good
{

    enum class ColliderShape : uint8 { Box, Sphere, Capsule };

    class Collider
    {
    public:
        virtual ~Collider() = default;

        virtual ColliderShape GetShape() const = 0;
        virtual AABB          GetAABB(const Transform& transform) const = 0;
        virtual bool          Raycast(const Ray& ray, const Transform& transform, RayHit& hit) const = 0;

        void SetOffset(const Vec3& o) { m_Offset = o; }
        const Vec3& GetOffset() const { return m_Offset; }

        bool IsTrigger() const { return m_IsTrigger; }
        void SetTrigger(bool v){ m_IsTrigger = v; }

    protected:
        Vec3 m_Offset{0,0,0};
        bool m_IsTrigger{false};
    };

} // namespace Good