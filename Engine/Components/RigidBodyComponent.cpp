#include "RigidBodyComponent.h"
#include "TransformComponent.h"
#include "../Objects/GameObject.h"

namespace Good
{

    const Vec3 RigidBodyComponent::s_Gravity(0.0f, -9.81f, 0.0f);

    void RigidBodyComponent::OnFixedUpdate(float32 dt)
    {
        if (m_Type != RigidBodyType::Dynamic) return;

        if (m_UseGravity) m_Force += s_Gravity * m_Mass;

        Vec3 accel   = m_Force * (1.0f / m_Mass);
        m_Velocity  += accel * dt;
        m_Velocity  *= (1.0f - m_Drag * dt);
        m_Force      = Vec3(0,0,0);

        if (m_Owner)
        {
            m_Owner->GetTransform().position += m_Velocity * dt;
        }
    }

} // namespace Good