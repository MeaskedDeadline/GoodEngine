#include "AnimationState.h"

namespace Good
{

    AnimationState::AnimationState(Ref<AnimationClip> clip)
        : m_Clip(std::move(clip)) {}

    void AnimationState::Update(float32 dt)
    {
        if (m_Status != AnimationStatus::Playing || !m_Clip) return;

        m_Time += dt * m_Speed;

        float32 duration = m_Clip->GetDuration();
        if (duration <= 0.0f) return;

        if (m_Clip->IsLooping())
        {
            while (m_Time >= duration) m_Time -= duration;
            while (m_Time <  0.0f)    m_Time += duration;
        }
        else
        {
            if (m_Time >= duration) { m_Time = duration; m_Status = AnimationStatus::Stopped; }
            if (m_Time <  0.0f)    { m_Time = 0.0f; }
        }
    }

    bool AnimationState::IsFinished() const
    {
        if (!m_Clip || m_Clip->IsLooping()) return false;
        return m_Time >= m_Clip->GetDuration();
    }

} // namespace Good