#pragma once
#include "Component.h"
#include "../Math/Vec3.h"
#include "../Math/Color.h"

namespace Good
{

    class ParticleComponent final : public Component
    {
    public:
        const char* GetTypeName() const override { return "ParticleComponent"; }

        void SetEmissionRate(float32 r)     { m_EmissionRate = r;     }
        void SetLifetime(float32 l)         { m_Lifetime = l;         }
        void SetStartColor(const Color& c)  { m_StartColor = c;       }
        void SetEndColor(const Color& c)    { m_EndColor = c;         }
        void SetStartSize(float32 s)        { m_StartSize = s;        }
        void SetEndSize(float32 s)          { m_EndSize = s;          }
        void SetStartSpeed(float32 s)       { m_StartSpeed = s;       }
        void SetGravityMultiplier(float32 g){ m_GravityMult = g;      }
        void SetMaxParticles(uint32 n)      { m_MaxParticles = n;     }

        void Play()  { m_Emitting = true;  }
        void Stop()  { m_Emitting = false; }
        bool IsEmitting() const { return m_Emitting; }

        void OnStart()           override { Play(); }
        void OnUpdate(float32 dt) override;

    private:
        float32 m_EmissionRate{10.0f};
        float32 m_Lifetime{2.0f};
        Color   m_StartColor{1,1,1,1};
        Color   m_EndColor{1,1,1,0};
        float32 m_StartSize{0.1f};
        float32 m_EndSize{0.0f};
        float32 m_StartSpeed{1.0f};
        float32 m_GravityMult{1.0f};
        uint32  m_MaxParticles{1000};
        bool    m_Emitting{false};
        float32 m_EmitTimer{0.0f};
    };

} // namespace Good