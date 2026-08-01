#include "RigidBody.h"
#include "Math/MathCommon.h"

namespace Good
{

    const Vec3 RigidBody::Gravity(0.0f, -9.81f, 0.0f);

    void RigidBody::Integrate(Vec3& position, Quat& rotation, float32 dt)
    {
        if (m_Type != BodyType::Dynamic)
        {
            m_Force  = Vec3(0,0,0);
            m_Torque = Vec3(0,0,0);
            return;
        }

        if (m_UseGravity) m_Force += Gravity * m_Mass;

        Vec3 accel    = m_Force * (1.0f / m_Mass);
        m_Velocity   += accel * dt;

        float32 dragFactor = 1.0f - m_Drag * dt;
        if (dragFactor < 0.0f) dragFactor = 0.0f;
        m_Velocity *= dragFactor;

        position += m_Velocity * dt;

        Vec3 angAccel      = m_Torque * (1.0f / m_Mass);
        m_AngularVelocity += angAccel * dt;

        float32 angDragFactor = 1.0f - m_AngularDrag * dt;
        if (angDragFactor < 0.0f) angDragFactor = 0.0f;
        m_AngularVelocity *= angDragFactor;

        float32 angLen = m_AngularVelocity.Length();
        if (angLen > Math::EPSILON)
        {
            float32 angle = angLen * dt;
            Vec3    axis  = m_AngularVelocity / angLen;
            rotation      = Quat::FromAxisAngle(axis, angle) * rotation;
            rotation.Normalize();
        }

        m_Force  = Vec3(0,0,0);
        m_Torque = Vec3(0,0,0);
    }

} // namespace Good