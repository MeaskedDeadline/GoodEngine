#pragma once
#include "../Core/Core.h"

namespace Good
{

    struct AudioBus
    {
        std::string name;
        float32     volume{1.0f};
        bool        muted{false};
    };

    class AudioMixer : public NonCopyable
    {
    public:
        static AudioMixer& Get();

        void AddBus(const std::string& name, float32 volume = 1.0f);
        void SetBusVolume(const std::string& name, float32 volume);
        void MuteBus(const std::string& name, bool mute);
        float32 GetBusVolume(const std::string& name) const;
        bool IsBusMuted(const std::string& name) const;

        void SetMasterVolume(float32 v) { m_MasterVolume = v; }
        float32 GetMasterVolume()  const{ return m_MasterVolume; }

    private:
        AudioMixer() { AddBus("Master"); AddBus("Music"); AddBus("SFX"); AddBus("UI"); }

        std::unordered_map<std::string, AudioBus> m_Buses;
        float32 m_MasterVolume{1.0f};
    };

} // namespace Good