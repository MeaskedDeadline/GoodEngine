#pragma once
#include "Core/Core.h"
#include "Math/Vec3.h"
#include "Math/Quat.h"

namespace Good
{

    enum class BodyType : uint8 { Dynamic, Kinematic, Static };

    class RigidBody
    {
    public:
        RigidBody() = default;

        void SetMass(float32 m)        { m_Mass = m > 0.0f ? m : 0.001f; }
        void SetType(BodyType t)       { m_Type = t;        }
        void SetGravity(bool v)        { m_UseGravity = v;  }
        void SetDrag(float32 d)        { m_Drag = d;        }
        void SetAngularDrag(float32 d) { m_AngularDrag = d; }

        void SetVelocity(const Vec3& v)       { m_Velocity = v;        }
        void SetAngularVelocity(const Vec3& v){ m_AngularVelocity = v; }

        void AddForce(const Vec3& f)   { m_Force  += f; }
        void AddTorque(const Vec3& t)  { m_Torque += t; }
        void AddImpulse(const Vec3& i) { m_Velocity += i / m_Mass; }

        float32      GetMass()            const { return m_Mass;            }
        BodyType     GetBodyType()        const { return m_Type;            }
        const Vec3&  GetVelocity()        const { return m_Velocity;        }
        const Vec3&  GetAngularVelocity() const { return m_AngularVelocity; }
        bool         UsesGravity()        const { return m_UseGravity;      }

        void Integrate(Vec3& position, Quat& rotation, float32 dt);

        static const Vec3 Gravity;

    private:
        BodyType m_Type{BodyType::Dynamic};
        float32  m_Mass{1.0f};
        float32  m_Drag{0.01f};
        float32  m_AngularDrag{0.05f};
        bool     m_UseGravity{true};
        Vec3     m_Velocity{0,0,0};
        Vec3     m_AngularVelocity{0,0,0};
        Vec3     m_Force{0,0,0};
        Vec3     m_Torque{0,0,0};
    };

} // namespace Good