#pragma once
#include "Component.h"
#include "../Math/Transform.h"

namespace Good
{

    class TransformComponent final : public Component
    {
    public:
        const char* GetTypeName() const override { return "TransformComponent"; }

        Vec3& Position() { return m_Transform.position; }
        Quat& Rotation() { return m_Transform.rotation; }
        Vec3& Scale()    { return m_Transform.scale;    }

        const Vec3& Position() const { return m_Transform.position; }
        const Quat& Rotation() const { return m_Transform.rotation; }
        const Vec3& Scale()    const { return m_Transform.scale;    }

        Transform&       GetTransform()       { return m_Transform; }
        const Transform& GetTransform() const { return m_Transform; }

        Mat4 GetMatrix()     const { return m_Transform.ToMatrix(); }
        Vec3 Forward()       const { return m_Transform.Forward(); }
        Vec3 Up()            const { return m_Transform.Up(); }
        Vec3 Right()         const { return m_Transform.Right(); }

    private:
        Transform m_Transform;
    };

} // namespace Good