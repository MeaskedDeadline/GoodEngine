#include "ParticleEmitter.h"
#include "../Utilities/Random.h"
#include "../Math/MathCommon.h"

namespace Good
{

ParticleEmitter::ParticleEmitter() = default;

Particle ParticleEmitter::Emit(const Vec3& position)
{
    Particle p;
    p.position        = GetEmitPosition(position);
    p.velocity        = GetEmitVelocity();
    p.acceleration    = m_Config.gravity * m_Config.gravityScale;
    p.startColor      = m_Config.startColor;
    p.endColor        = m_Config.endColor;
    p.color           = m_Config.startColor;
    p.startSize       = m_Config.startSize + Random::Float(-m_Config.sizeVariance, m_Config.sizeVariance);
    p.endSize         = m_Config.endSize;
    p.size            = p.startSize;
    p.lifetime        = m_Config.lifetime + Random::Float(-m_Config.lifetimeVariance, m_Config.lifetimeVariance);
    p.lifetime        = Math::Max(0.01f, p.lifetime);
    p.age             = 0.0f;
    p.rotation        = m_Config.startRotation;
    p.angularVelocity = m_Config.angularVelocity;
    p.alive           = true;
    return p;
}

Vec3 ParticleEmitter::GetEmitPosition(const Vec3& origin) const
{
    switch (m_Config.shape)
    {
        case EmitterShape::Sphere:
        {
            Vec3 dir(Random::Float(-1,1), Random::Float(-1,1), Random::Float(-1,1));
            dir = dir.Normalized() * Random::Float(0, m_Config.shapeSize.x);
            return origin + dir;
        }
        case EmitterShape::Box:
        {
            return origin + Vec3(
                Random::Float(-m_Config.shapeSize.x, m_Config.shapeSize.x) * 0.5f,
                Random::Float(-m_Config.shapeSize.y, m_Config.shapeSize.y) * 0.5f,
                Random::Float(-m_Config.shapeSize.z, m_Config.shapeSize.z) * 0.5f);
        }
        case EmitterShape::Circle:
        {
            float32 angle = Random::Float(0, Math::TWO_PI);
            float32 r     = Random::Float(0, m_Config.shapeSize.x);
            return origin + Vec3(Math::Cos(angle)*r, 0, Math::Sin(angle)*r);
        }
        default: return origin;
    }
}

Vec3 ParticleEmitter::GetEmitVelocity() const
{
    Vec3 vel = m_Config.startVelocity;
    float32 var = m_Config.velocityVariance;
    vel += Vec3(Random::Float(-var,var), Random::Float(-var,var), Random::Float(-var,var));

    if (m_Config.shape == EmitterShape::Cone)
    {
        float32 angle = Math::ToRadians(m_Config.coneAngle * Random::Float(0.0f, 1.0f));
        float32 phi   = Random::Float(0, Math::TWO_PI);
        vel = Vec3(Math::Sin(angle)*Math::Cos(phi),
                   Math::Cos(angle),
                   Math::Sin(angle)*Math::Sin(phi)) * vel.Length();
    }

    return vel;
}

} // namespace Good