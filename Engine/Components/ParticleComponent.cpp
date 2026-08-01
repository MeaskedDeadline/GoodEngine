#include "ParticleComponent.h"

namespace Good
{

    void ParticleComponent::OnUpdate(float32 dt)
    {
        if (!m_Emitting) return;

        m_EmitTimer += dt;
        float32 interval = 1.0f / m_EmissionRate;

        while (m_EmitTimer >= interval)
        {
            m_EmitTimer -= interval;
            // Частицы создаются через ParticleSystem (позже)
        }
    }

} // namespace Good