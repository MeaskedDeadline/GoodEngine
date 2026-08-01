#pragma once
#include "../Core/Core.h"
#include "AudioSource.h"
#include "AudioListener.h"
#include "AudioMixer.h"

namespace Good
{

    class AudioSystem : public NonCopyable
    {
    public:
        static AudioSystem& Get();

        bool Initialize();
        void Shutdown();
        void Update(float32 dt);

        Ref<AudioSource> CreateSource();
        void             DestroySource(Ref<AudioSource> source);

        AudioListener& GetListener() { return AudioListener::Get(); }
        AudioMixer&    GetMixer()    { return AudioMixer::Get();    }

        bool IsInitialized() const { return m_Initialized; }

    private:
        AudioSystem() = default;

        std::vector<Ref<AudioSource>> m_Sources;
        bool m_Initialized{false};
    };

} // namespace Good