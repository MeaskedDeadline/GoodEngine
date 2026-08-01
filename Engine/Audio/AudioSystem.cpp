#include "AudioSystem.h"
#include "../Core/Log.h"

namespace Good
{

    AudioSystem& AudioSystem::Get()
    {
        static AudioSystem s_Instance;
        return s_Instance;
    }

    bool AudioSystem::Initialize()
    {
        if (m_Initialized) return true;
        m_Initialized = true;
        GOOD_LOG_INFO("Audio", "AudioSystem initialized (stub)");
        return true;
    }

    void AudioSystem::Shutdown()
    {
        m_Sources.clear();
        m_Initialized = false;
        GOOD_LOG_INFO("Audio", "AudioSystem shutdown");
    }

    void AudioSystem::Update(float32 dt)
    {
        for (auto& src : m_Sources)
        {
            if (!src || !src->IsPlaying()) continue;
            // Позже: микширование, 3D позиционирование, HRTF
        }
    }

    Ref<AudioSource> AudioSystem::CreateSource()
    {
        auto src = MakeRef<AudioSource>();
        m_Sources.push_back(src);
        return src;
    }

    void AudioSystem::DestroySource(Ref<AudioSource> source)
    {
        m_Sources.erase(
            std::remove(m_Sources.begin(), m_Sources.end(), source),
            m_Sources.end());
    }

} // namespace Good