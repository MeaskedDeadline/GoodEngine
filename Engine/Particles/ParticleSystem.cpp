#include "ParticleSystem.h"
#include "../Math/MathCommon.h"

namespace Good
{

    ParticleSystem::ParticleSystem()
    {
        m_Particles.reserve(1000);
    }

    void ParticleSystem::SetEmitter(const EmitterConfig& config)
    {
        m_Emitter.SetConfig(config);
        m_Particles.reserve(config.maxParticles);
    }

    void ParticleSystem::Update(float32 dt)
    {
        if (!m_Active) return;
        SpawnParticles(dt);
        UpdateParticles(dt);
        KillDeadParticles();
    }

    void ParticleSystem::SpawnParticles(float32 dt)
    {
        if (!m_Emitter.IsEmitting()) return;

        float32 rate = m_Emitter.GetConfig().emissionRate;
        uint32  max  = m_Emitter.GetConfig().maxParticles;

        m_EmitTimer += dt;
        float32 interval = 1.0f / rate;

        while (m_EmitTimer >= interval && m_Particles.size() < max)
        {
            m_EmitTimer -= interval;
            m_Particles.push_back(m_Emitter.Emit(m_Position));
        }
    }

    void ParticleSystem::UpdateParticles(float32 dt)
    {
        for (auto& p : m_Particles)
        {
            if (!p.alive) continue;

            p.age += dt;

            float32 t = p.GetLifeRatio();

            p.velocity += p.acceleration * dt;
            p.position += p.velocity * dt;

            p.color = Color(
                Math::Lerp(p.startColor.r, p.endColor.r, t),
                Math::Lerp(p.startColor.g, p.endColor.g, t),
                Math::Lerp(p.startColor.b, p.endColor.b, t),
                Math::Lerp(p.startColor.a, p.endColor.a, t));

            p.size     = Math::Lerp(p.startSize, p.endSize, t);
            p.rotation += p.angularVelocity * dt;

            if (p.age >= p.lifetime) p.alive = false;
        }
    }

    void ParticleSystem::KillDeadParticles()
    {
        m_Particles.erase(
            std::remove_if(m_Particles.begin(), m_Particles.end(),
                [](const Particle& p) { return !p.alive; }),
            m_Particles.end());
    }

    usize ParticleSystem::GetAliveCount() const
    {
        usize count = 0;
        for (auto& p : m_Particles) if (p.alive) ++count;
        return count;
    }

} // namespace Good