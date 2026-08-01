#pragma once
#include "../Core/Core.h"
#include "ParticleSystem.h"

namespace Good
{

    class ParticleEffect
    {
    public:
        ParticleEffect() = default;
        explicit ParticleEffect(const std::string& name);

        void AddSystem(Ref<ParticleSystem> system) { m_Systems.push_back(std::move(system)); }

        void Play();
        void Stop();
        void Update(float32 dt);
        void SetPosition(const Vec3& pos);

        bool IsPlaying() const { return m_Playing; }
        const std::string& GetName() const { return m_Name; }

        static Ref<ParticleEffect> CreateExplosion(const Vec3& position);
        static Ref<ParticleEffect> CreateFire(const Vec3& position);
        static Ref<ParticleEffect> CreateSmoke(const Vec3& position);

    private:
        std::string                    m_Name{"Effect"};
        std::vector<Ref<ParticleSystem>> m_Systems;
        bool                           m_Playing{false};
    };

} // namespace Good