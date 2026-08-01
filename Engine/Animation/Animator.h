#pragma once
#include "Core/Core.h"
#include "AnimationState.h"
#include "Skeleton.h"
#include "Math/Mat4.h"

namespace Good
{

    class Animator
    {
    public:
        Animator() = default;

        void SetSkeleton(Ref<Skeleton> skeleton) { m_Skeleton = std::move(skeleton); }

        void Play(Ref<AnimationClip> clip, bool loop = true);
        void Stop();
        void Pause();
        void SetSpeed(float32 s);
        void Update(float32 dt);

        const std::vector<Mat4>& GetBoneMatrices() const { return m_BoneMatrices; }

        bool               IsPlaying()      const;
        Ref<AnimationClip> GetCurrentClip() const;

    private:
        Ref<Skeleton>     m_Skeleton;
        AnimationState    m_CurrentState;
        std::vector<Mat4> m_BoneMatrices;
    };

} // namespace Good