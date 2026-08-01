#pragma once
#include "Component.h"
#include "../Math/Vec3.h"

namespace Good
{

    enum class ColliderType : uint8 { Box, Sphere, Capsule, Mesh };

    class ColliderComponent : public Component
    {
    public:
        const char* GetTypeName() const override { return "ColliderComponent"; }

        void SetColliderType(ColliderType t) { m_ColliderType = t;  }
        ColliderType GetColliderType() const { return m_ColliderType; }

        void SetOffset(const Vec3& o)    { m_Offset = o;    }
        void SetIsTrigger(bool v)        { m_IsTrigger = v; }

        const Vec3& GetOffset() const    { return m_Offset;    }
        bool        IsTrigger() const    { return m_IsTrigger; }

    protected:
        ColliderType m_ColliderType{ColliderType::Box};
        Vec3         m_Offset{0,0,0};
        bool         m_IsTrigger{false};
    };

} // namespace Good