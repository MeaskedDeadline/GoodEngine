#pragma once
#include "Core/Core.h"
#include "AnimationClip.h"

namespace Good
{

    enum class AnimationStatus : uint8 { Stopped, Playing, Paused };

    class AnimationState
    {
    public:
        AnimationState() = default;
        explicit AnimationState(Ref<AnimationClip> clip);

        void Play()  { m_Status = AnimationStatus::Playing; }
        void Stop()  { m_Status = AnimationStatus::Stopped; m_Time = 0.0f; }
        void Pause() { m_Status = AnimationStatus::Paused;  }

        void Update(float32 dt);

        Ref<AnimationClip> GetClip()   const { return m_Clip;  }
        float32            GetTime()   const { return m_Time;  }
        float32            GetSpeed()  const { return m_Speed; }
        AnimationStatus    GetStatus() const { return m_Status;}

        void SetSpeed(float32 s) { m_Speed = s; }
        void SetTime(float32 t)  { m_Time  = t; }

        bool IsPlaying()  const { return m_Status == AnimationStatus::Playing; }
        bool IsFinished() const;

    private:
        Ref<AnimationClip> m_Clip;
        float32            m_Time{0.0f};
        float32            m_Speed{1.0f};
        AnimationStatus    m_Status{AnimationStatus::Stopped};
    };

} // namespace Good