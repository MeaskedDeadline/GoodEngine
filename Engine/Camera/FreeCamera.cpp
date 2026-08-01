#include "FreeCamera.h"
#include "../Input/Input.h"
#include "../Input/KeyCodes.h"
#include "../Input/MouseCodes.h"
#include "../Math/MathCommon.h"

namespace Good
{

    FreeCamera::FreeCamera() = default;

    void FreeCamera::OnUpdate(Camera& camera, float32 dt)
    {
        auto& input = Input::Get();

        float32 speed = m_Speed;
        if (input.IsKeyDown(Key::LeftShift)) speed *= m_SprintMult;

        Transform& tr = camera.GetTransform();

        Vec3 fwd   = tr.Forward();
        Vec3 right = tr.Right();

        if (input.IsKeyDown(Key::W)) tr.position += fwd   * speed * dt;
        if (input.IsKeyDown(Key::S)) tr.position -= fwd   * speed * dt;
        if (input.IsKeyDown(Key::D)) tr.position += right * speed * dt;
        if (input.IsKeyDown(Key::A)) tr.position -= right * speed * dt;
        if (input.IsKeyDown(Key::E)) tr.position += Vec3(0,1,0) * speed * dt;
        if (input.IsKeyDown(Key::Q)) tr.position -= Vec3(0,1,0) * speed * dt;

        if (input.IsMouseButtonDown(MouseButton::Right))
        {
            float32 dx = input.GetMouseDeltaX() * m_Sensitivity;
            float32 dy = input.GetMouseDeltaY() * m_Sensitivity;

            m_Yaw   -= dx;
            m_Pitch -= dy;
            m_Pitch  = Math::Clamp(m_Pitch, -89.0f, 89.0f);

            tr.rotation = Quat::FromEuler(m_Pitch, m_Yaw, 0.0f);
        }
    }

} // namespace Good