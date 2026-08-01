#pragma once
#include "../Core/Core.h"
#include "../Math/Vec3.h"
#include "../Math/Color.h"
#include "Particle.h"

namespace Good
{

    enum class EmitterShape : uint8 { Point, Sphere, Box, Cone, Circle };

    struct EmitterConfig
    {
        EmitterShape shape{EmitterShape::Point};
        Vec3         shapeSize{1,1,1};
        float32      coneAngle{30.0f};

        float32      emissionRate{10.0f};
        float32      lifetime{2.0f};
        float32      lifetimeVariance{0.5f};

        Vec3         startVelocity{0,1,0};
        float32      velocityVariance{0.5f};
        Vec3         gravity{0,-9.81f,0};
        float32      gravityScale{0.0f};

        Color        startColor{1,1,1,1};
        Color        endColor{1,1,1,0};

        float32      startSize{0.1f};
        float32      endSize{0.0f};
        float32      sizeVariance{0.02f};

        float32      startRotation{0.0f};
        float32      angularVelocity{0.0f};

        uint32       maxParticles{1000};
    };

    class ParticleEmitter
    {
    public:
        ParticleEmitter();

        void SetConfig(const EmitterConfig& cfg) { m_Config = cfg; }
        const EmitterConfig& GetConfig()   const { return m_Config; }

        void Start()  { m_Emitting = true; }
        void Stop()   { m_Emitting = false; }
        bool IsEmitting() const { return m_Emitting; }

        Particle Emit(const Vec3& position);

    private:
        Vec3 GetEmitPosition(const Vec3& origin) const;
        Vec3 GetEmitVelocity() const;

        EmitterConfig m_Config;
        bool          m_Emitting{true};
    };

} // namespace Good