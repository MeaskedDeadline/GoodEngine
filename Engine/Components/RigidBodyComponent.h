#pragma once
#include "Component.h"
#include "../Math/Vec3.h"

namespace Good
{

    enum class RigidBodyType : uint8 { Dynamic, Kinematic, Static };

    class RigidBodyComponent final : public Component
    {
    public:
        const char* GetTypeName() const override { return "RigidBodyComponent"; }

        void SetMass(float32 m)         { m_Mass = m;            }
        void SetType(RigidBodyType t)   { m_Type = t;            }
        void SetGravity(bool v)         { m_UseGravity = v;      }
        void SetDrag(float32 d)         { m_Drag = d;            }
        void SetAngularDrag(float32 d)  { m_AngularDrag = d;     }

        void AddForce(const Vec3& f)    { m_Force     += f;      }
        void AddImpulse(const Vec3& i)  { m_Velocity  += i / (m_Mass > 0 ? m_Mass : 1.0f); }
        void SetVelocity(const Vec3& v) { m_Velocity = v;        }

        float32       GetMass()         const { return m_Mass;       }
        RigidBodyType GetBodyType()     const { return m_Type;       }
        const Vec3&   GetVelocity()     const { return m_Velocity;   }
        bool          UsesGravity()     const { return m_UseGravity; }

        void OnFixedUpdate(float32 dt) override;

    private:
        RigidBodyType m_Type{RigidBodyType::Dynamic};
        float32  m_Mass{1.0f};
        float32  m_Drag{0.01f};
        float32  m_AngularDrag{0.05f};
        bool     m_UseGravity{true};
        Vec3     m_Velocity{0,0,0};
        Vec3     m_Force{0,0,0};

        static const Vec3 s_Gravity;
    };

} // namespace Good