#pragma once
#include "../Core/Core.h"

namespace Good
{

    class AudioDevice : public NonCopyable
    {
    public:
        static AudioDevice& Get();

        bool Initialize(uint32 sampleRate = 44100, uint32 bufferSize = 1024);
        void Shutdown();

        uint32 GetSampleRate() const { return m_SampleRate; }
        uint32 GetBufferSize() const { return m_BufferSize; }
        bool   IsOpen()        const { return m_Open;       }

    private:
        AudioDevice() = default;

        uint32 m_SampleRate{44100};
        uint32 m_BufferSize{1024};
        bool   m_Open{false};
    };

} // namespace Good