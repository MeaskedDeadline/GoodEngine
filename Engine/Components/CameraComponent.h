#pragma once
#include "Component.h"
#include "../Math/Mat4.h"

namespace Good
{

    enum class ProjectionType : uint8 { Perspective, Orthographic };

    class CameraComponent final : public Component
    {
    public:
        const char* GetTypeName() const override { return "CameraComponent"; }

        void SetProjection(ProjectionType type) { m_Type = type; }
        void SetFOV(float32 fov)                { m_FOV = fov;   }
        void SetNear(float32 n)                 { m_Near = n;    }
        void SetFar(float32 f)                  { m_Far = f;     }
        void SetAspect(float32 a)               { m_Aspect = a;  }
        void SetOrthoSize(float32 s)            { m_OrthoSize = s;}

        Mat4 GetProjectionMatrix() const;
        Mat4 GetViewMatrix() const;

        bool IsPrimary() const        { return m_Primary; }
        void SetPrimary(bool v)       { m_Primary = v; }

    private:
        ProjectionType m_Type{ProjectionType::Perspective};
        float32 m_FOV{60.0f};
        float32 m_Near{0.1f};
        float32 m_Far{1000.0f};
        float32 m_Aspect{16.0f/9.0f};
        float32 m_OrthoSize{10.0f};
        bool    m_Primary{false};
    };

} // namespace Good