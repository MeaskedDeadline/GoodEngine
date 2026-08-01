#pragma once
#include "../Core/Core.h"
#include "../Math/Vec3.h"
#include "../Math/Mat4.h"
#include "Particle.h"
#include "ParticleEmitter.h"

namespace Good
{

    class ParticleSystem
    {
    public:
        ParticleSystem();

        void SetEmitter(const EmitterConfig& config);
        void SetPosition(const Vec3& pos) { m_Position = pos; }

        void Play()  { m_Active = true;  m_Emitter.Start(); }
        void Stop()  { m_Active = false; m_Emitter.Stop();  }
        void Clear() { m_Particles.clear(); }

        void Update(float32 dt);

        bool  IsActive()       const { return m_Active; }
        usize GetAliveCount()  const;

        const std::vector<Particle>& GetParticles() const { return m_Particles; }

    private:
        void SpawnParticles(float32 dt);
        void UpdateParticles(float32 dt);
        void KillDeadParticles();

        ParticleEmitter      m_Emitter;
        std::vector<Particle> m_Particles;
        Vec3                  m_Position{0,0,0};
        float32               m_EmitTimer{0.0f};
        bool                  m_Active{false};
    };

} // namespace Good