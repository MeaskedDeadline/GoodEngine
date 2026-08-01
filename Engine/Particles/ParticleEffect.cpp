#include "ParticleEffect.h"

namespace Good
{

ParticleEffect::ParticleEffect(const std::string& name) : m_Name(name) {}

void ParticleEffect::Play()
{
    m_Playing = true;
    for (auto& sys : m_Systems) if (sys) sys->Play();
}

void ParticleEffect::Stop()
{
    m_Playing = false;
    for (auto& sys : m_Systems) if (sys) sys->Stop();
}

void ParticleEffect::Update(float32 dt)
{
    if (!m_Playing) return;
    for (auto& sys : m_Systems) if (sys) sys->Update(dt);
}

void ParticleEffect::SetPosition(const Vec3& pos)
{
    for (auto& sys : m_Systems) if (sys) sys->SetPosition(pos);
}

Ref<ParticleEffect> ParticleEffect::CreateExplosion(const Vec3& position)
{
    auto effect = MakeRef<ParticleEffect>("Explosion");

    EmitterConfig cfg;
    cfg.shape         = EmitterShape::Sphere;
    cfg.shapeSize     = Vec3(0.1f);
    cfg.emissionRate  = 500.0f;
    cfg.lifetime      = 0.5f;
    cfg.startVelocity = Vec3(0,2,0);
    cfg.velocityVariance = 3.0f;
    cfg.startColor    = Color(1.0f, 0.5f, 0.1f, 1.0f);
    cfg.endColor      = Color(0.5f, 0.1f, 0.0f, 0.0f);
    cfg.startSize     = 0.3f;
    cfg.endSize       = 0.01f;
    cfg.maxParticles  = 500;

    auto sys = MakeRef<ParticleSystem>();
    sys->SetEmitter(cfg);
    sys->SetPosition(position);
    effect->AddSystem(sys);

    return effect;
}

Ref<ParticleEffect> ParticleEffect::CreateFire(const Vec3& position)
{
    auto effect = MakeRef<ParticleEffect>("Fire");

    EmitterConfig cfg;
    cfg.shape         = EmitterShape::Circle;
    cfg.shapeSize     = Vec3(0.3f);
    cfg.emissionRate  = 50.0f;
    cfg.lifetime      = 1.5f;
    cfg.startVelocity = Vec3(0,2,0);
    cfg.velocityVariance = 0.3f;
    cfg.gravityScale  = -0.1f;
    cfg.startColor    = Color(1.0f, 0.4f, 0.0f, 1.0f);
    cfg.endColor      = Color(0.5f, 0.0f, 0.0f, 0.0f);
    cfg.startSize     = 0.2f;
    cfg.endSize       = 0.4f;
    cfg.maxParticles  = 300;

    auto sys = MakeRef<ParticleSystem>();
    sys->SetEmitter(cfg);
    sys->SetPosition(position);
    effect->AddSystem(sys);

    return effect;
}

Ref<ParticleEffect> ParticleEffect::CreateSmoke(const Vec3& position)
{
    auto effect = MakeRef<ParticleEffect>("Smoke");

    EmitterConfig cfg;
    cfg.shape         = EmitterShape::Sphere;
    cfg.shapeSize     = Vec3(0.2f);
    cfg.emissionRate  = 20.0f;
    cfg.lifetime      = 3.0f;
    cfg.startVelocity = Vec3(0,0.5f,0);
    cfg.velocityVariance = 0.2f;
    cfg.startColor    = Color(0.5f, 0.5f, 0.5f, 0.5f);
    cfg.endColor      = Color(0.2f, 0.2f, 0.2f, 0.0f);
    cfg.startSize     = 0.3f;
    cfg.endSize       = 1.0f;
    cfg.maxParticles  = 200;

    auto sys = MakeRef<ParticleSystem>();
    sys->SetEmitter(cfg);
    sys->SetPosition(position);
    effect->AddSystem(sys);

    return effect;
}

} // namespace Good