#include "Camera.h"
#include "../Math/MathCommon.h"
#include "../Math/Vec4.h"

namespace Good
{

    Camera::Camera()
    {
        m_Transform.position = Vec3(0, 0, 5);
    }

    Mat4 Camera::GetViewMatrix() const { return m_Transform.ToViewMatrix(); }

    Mat4 Camera::GetProjectionMatrix() const
    {
        if (m_Config.mode == CameraMode::Perspective)
            return Mat4::Perspective(Math::ToRadians(m_Config.fovDegrees),
                                     m_Config.aspectRatio,
                                     m_Config.nearPlane,
                                     m_Config.farPlane);

        float32 h = m_Config.orthoSize * 0.5f;
        float32 w = h * m_Config.aspectRatio;
        return Mat4::Ortho(-w, w, -h, h, m_Config.nearPlane, m_Config.farPlane);
    }

    Mat4 Camera::GetViewProjection() const
    {
        return GetProjectionMatrix() * GetViewMatrix();
    }

    Vec3 Camera::ScreenToWorldDirection(float32 ndcX, float32 ndcY) const
    {
        Mat4 invVP = (GetProjectionMatrix() * GetViewMatrix()).Inverted();
        Vec4 clip(ndcX, ndcY, -1.0f, 1.0f);
        Vec4 eye = invVP * clip;
        return Vec3(eye.x, eye.y, eye.z).Normalized();
    }

} // namespace Good