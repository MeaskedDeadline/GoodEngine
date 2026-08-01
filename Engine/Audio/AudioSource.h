#pragma once
#include "../Core/Core.h"
#include "../Math/Vec3.h"
#include "AudioClip.h"

namespace Good
{

    class AudioSource
    {
    public:
        AudioSource() = default;

        void SetClip(Ref<AudioClip> clip) { m_Clip = std::move(clip); }
        void SetVolume(float32 v)         { m_Volume  = Math::Clamp(v, 0.0f, 1.0f); }
        void SetPitch(float32 p)          { m_Pitch   = p; }
        void SetLooping(bool l)           { m_Looping = l; }
        void Set3D(bool v)                { m_3D      = v; }
        void SetPosition(const Vec3& pos) { m_Position = pos; }
        void SetMinDistance(float32 d)    { m_MinDist  = d; }
        void SetMaxDistance(float32 d)    { m_MaxDist  = d; }

        void Play();
        void Stop();
        void Pause();
        void Resume();

        bool IsPlaying() const { return m_Playing; }
        bool IsPaused()  const { return m_Paused;  }
        bool IsLooping() const { return m_Looping; }

        Ref<AudioClip> GetClip()   const { return m_Clip;   }
        float32 GetVolume()        const { return m_Volume;  }
        float32 GetPitch()         const { return m_Pitch;   }

    private:
        Ref<AudioClip> m_Clip;
        Vec3           m_Position{0,0,0};
        float32        m_Volume{1.0f};
        float32        m_Pitch{1.0f};
        float32        m_MinDist{1.0f};
        float32        m_MaxDist{50.0f};
        bool           m_Looping{false};
        bool           m_3D{true};
        bool           m_Playing{false};
        bool           m_Paused{false};
    };

} // namespace Good