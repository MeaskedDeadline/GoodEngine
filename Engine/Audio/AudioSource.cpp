#include "AudioSource.h"
#include "../Core/Log.h"
#include "../Math/MathCommon.h"

namespace Good
{

    void AudioSource::Play()
    {
        if (!m_Clip || !m_Clip->IsLoaded())
        {
            GOOD_LOG_WARN("AudioSource", "No clip loaded");
            return;
        }
        m_Playing = true;
        m_Paused  = false;
    }

    void AudioSource::Stop()
    {
        m_Playing = false;
        m_Paused  = false;
    }

    void AudioSource::Pause()
    {
        if (m_Playing) { m_Playing = false; m_Paused = true; }
    }

    void AudioSource::Resume()
    {
        if (m_Paused) { m_Playing = true; m_Paused = false; }
    }

} // namespace Good