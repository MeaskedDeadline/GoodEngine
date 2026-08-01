#include "OrbitCamera.h"
#include "../Input/Input.h"
#include "../Input/MouseCodes.h"
#include "../Math/MathCommon.h"

namespace Good
{

    OrbitCamera::OrbitCamera() = default;

    void OrbitCamera::OnUpdate(Camera& camera, float32 dt)
    {
        auto& input = Input::Get();

        if (input.IsMouseButtonDown(MouseButton::Right))
        {
            float32 dx = input.GetMouseDeltaX() * m_Sensitivity;
            float32 dy = input.GetMouseDeltaY() * m_Sensitivity;
            m_Yaw   += dx;
            m_Pitch -= dy;
            m_Pitch  = Math::Clamp(m_Pitch, -89.0f, 89.0f);
        }

        float32 scroll = input.GetScrollY();
        m_Distance -= scroll * 0.5f;
        m_Distance  = Math::Clamp(m_Distance, m_MinDist, m_MaxDist);

        float32 yawRad   = Math::ToRadians(m_Yaw);
        float32 pitchRad = Math::ToRadians(m_Pitch);

        Vec3 offset(
            Math::Cos(pitchRad) * Math::Sin(yawRad) * m_Distance,
            Math::Sin(pitchRad) * m_Distance,
            Math::Cos(pitchRad) * Math::Cos(yawRad) * m_Distance);

        Transform& tr  = camera.GetTransform();
        tr.position    = m_Target + offset;
        tr.LookAt(m_Target);
    }

} // namespace Good