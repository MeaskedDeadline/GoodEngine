#pragma once
#include "Component.h"

namespace Good
{

    class AnimatorComponent final : public Component
    {
    public:
        const char* GetTypeName() const override { return "AnimatorComponent"; }

        void SetClip(const std::string& clip)       { m_CurrentClip = clip; }
        const std::string& GetCurrentClip()   const { return m_CurrentClip; }

        void Play(const std::string& clip)    { m_CurrentClip = clip; m_Playing = true;  m_Time = 0.0f; }
        void Stop()                           { m_Playing = false; }
        void SetSpeed(float32 speed)          { m_Speed = speed; }
        void SetLooping(bool v)               { m_Looping = v;   }

        bool    IsPlaying() const { return m_Playing; }
        float32 GetTime()   const { return m_Time;    }
        float32 GetSpeed()  const { return m_Speed;   }

        void OnUpdate(float32 dt) override
        {
            if (m_Playing) m_Time += dt * m_Speed;
        }

    private:
        std::string m_CurrentClip;
        float32     m_Time{0.0f};
        float32     m_Speed{1.0f};
        bool        m_Playing{false};
        bool        m_Looping{true};
    };

} // namespace Good