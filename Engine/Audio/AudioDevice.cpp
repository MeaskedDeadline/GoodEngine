#include "AudioDevice.h"
#include "../Core/Log.h"

namespace Good
{

    AudioDevice& AudioDevice::Get()
    {
        static AudioDevice s_Instance;
        return s_Instance;
    }

    bool AudioDevice::Initialize(uint32 sampleRate, uint32 bufferSize)
    {
        m_SampleRate = sampleRate;
        m_BufferSize = bufferSize;
        m_Open       = true;
        GOOD_LOG_INFO("AudioDevice", "Initialized ({}Hz, buf={})", sampleRate, bufferSize);
        return true;
    }

    void AudioDevice::Shutdown()
    {
        m_Open = false;
        GOOD_LOG_INFO("AudioDevice", "Shutdown");
    }

} // namespace Good