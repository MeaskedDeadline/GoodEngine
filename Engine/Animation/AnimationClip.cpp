#include "AnimationClip.h"
#include <algorithm>

namespace Good
{

    AnimationClip::AnimationClip(const std::string& name) : m_Name(name) {}

    void AnimationClip::AddBoneAnimation(BoneAnimation anim)
    {
        float32 dur = std::max({
            anim.positionTrack.GetDuration(),
            anim.rotationTrack.GetDuration(),
            anim.scaleTrack.GetDuration()});

        m_Duration = std::max(m_Duration, dur);
        m_BoneAnims.push_back(std::move(anim));
    }

} // namespace Good