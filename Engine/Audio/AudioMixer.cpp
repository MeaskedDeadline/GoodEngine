#include "AudioMixer.h"

namespace Good
{

    AudioMixer& AudioMixer::Get()
    {
        static AudioMixer s_Instance;
        return s_Instance;
    }

    void AudioMixer::AddBus(const std::string& name, float32 volume)
    {
        m_Buses[name] = {name, volume, false};
    }

    void AudioMixer::SetBusVolume(const std::string& name, float32 volume)
    {
        auto it = m_Buses.find(name);
        if (it != m_Buses.end()) it->second.volume = volume;
    }

    void AudioMixer::MuteBus(const std::string& name, bool mute)
    {
        auto it = m_Buses.find(name);
        if (it != m_Buses.end()) it->second.muted = mute;
    }

    float32 AudioMixer::GetBusVolume(const std::string& name) const
    {
        auto it = m_Buses.find(name);
        return (it != m_Buses.end()) ? it->second.volume : 1.0f;
    }

    bool AudioMixer::IsBusMuted(const std::string& name) const
    {
        auto it = m_Buses.find(name);
        return (it != m_Buses.end()) ? it->second.muted : false;
    }

} // namespace Good