#include "Animator.h"

namespace Good
{

    void Animator::Play(Ref<AnimationClip> clip, bool loop)
    {
        if (clip) clip->SetLooping(loop);
        m_CurrentState = AnimationState(clip);
        m_CurrentState.Play();
    }

    void Animator::Stop()         { m_CurrentState.Stop();  }
    void Animator::Pause()        { m_CurrentState.Pause(); }
    void Animator::SetSpeed(float32 s) { m_CurrentState.SetSpeed(s); }

    void Animator::Update(float32 dt)
    {
        m_CurrentState.Update(dt);

        if (!m_Skeleton) return;

        auto clip = m_CurrentState.GetClip();
        if (!clip) return;

        usize boneCount = m_Skeleton->GetBoneCount();

        std::vector<Mat4> worldMatrices;
        m_Skeleton->ComputeWorldMatrices(worldMatrices);

        m_BoneMatrices.resize(boneCount);

        for (usize i = 0; i < boneCount; ++i)
        {
            Mat4 invBind = m_Skeleton->GetBone(static_cast<int32>(i)).GetInverseBindPose();
            m_BoneMatrices[i] = worldMatrices[i] * invBind;
        }
    }

    bool Animator::IsPlaying() const
    {
        return m_CurrentState.IsPlaying();
    }

    Ref<AnimationClip> Animator::GetCurrentClip() const
    {
        return m_CurrentState.GetClip();
    }

} // namespace Good