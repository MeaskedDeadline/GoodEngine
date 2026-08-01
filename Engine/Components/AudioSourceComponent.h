#pragma once
#include "Component.h"

namespace Good
{

    class AudioSourceComponent final : public Component
    {
    public:
        const char* GetTypeName() const override { return "AudioSourceComponent"; }

        void SetClipPath(const std::string& path) { m_ClipPath = path; }
        void SetVolume(float32 v)                 { m_Volume = v;      }
        void SetPitch(float32 p)                  { m_Pitch = p;       }
        void SetLooping(bool v)                   { m_Looping = v;     }
        void SetPlayOnStart(bool v)               { m_PlayOnStart = v; }
        void Set3D(bool v)                        { m_3D = v;          }
        void SetMinDistance(float32 d)            { m_MinDist = d;     }
        void SetMaxDistance(float32 d)            { m_MaxDist = d;     }

        void Play()  { m_Playing = true;  }
        void Stop()  { m_Playing = false; }
        void Pause() { m_Playing = false; }

        bool IsPlaying() const { return m_Playing; }

        void OnStart() override { if (m_PlayOnStart) Play(); }

    private:
        std::string m_ClipPath;
        float32     m_Volume{1.0f};
        float32     m_Pitch{1.0f};
        bool        m_Looping{false};
        bool        m_PlayOnStart{false};
        bool        m_3D{true};
        float32     m_MinDist{1.0f};
        float32     m_MaxDist{50.0f};
        bool        m_Playing{false};
    };

} // namespace Good