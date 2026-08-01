#include "CameraComponent.h"
#include "../Math/MathCommon.h"

namespace Good
{

    Mat4 CameraComponent::GetProjectionMatrix() const
    {
        if (m_Type == ProjectionType::Perspective)
            return Mat4::Perspective(Math::ToRadians(m_FOV), m_Aspect, m_Near, m_Far);

        float32 half  = m_OrthoSize * 0.5f;
        float32 halfW = half * m_Aspect;
        return Mat4::Ortho(-halfW, halfW, -half, half, m_Near, m_Far);
    }

    Mat4 CameraComponent::GetViewMatrix() const
    {
        if (!m_Owner) return Mat4::Identity();
        return Mat4::Identity();
    }

} // namespace Good