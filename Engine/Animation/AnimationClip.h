#pragma once
#include "Core/Core.h"
#include "AnimationTrack.h"

namespace Good
{

    struct BoneAnimation
    {
        std::string name;
        Vec3Track   positionTrack;
        QuatTrack   rotationTrack;
        Vec3Track   scaleTrack;
    };

    class AnimationClip
    {
    public:
        AnimationClip() = default;
        explicit AnimationClip(const std::string& name);

        const std::string& GetName()     const { return m_Name;     }
        float32            GetDuration() const { return m_Duration; }
        bool               IsLooping()   const { return m_Looping;  }

        void SetLooping(bool l)     { m_Looping  = l; }
        void SetDuration(float32 d) { m_Duration = d; }

        void AddBoneAnimation(BoneAnimation anim);
        const std::vector<BoneAnimation>& GetBoneAnimations() const { return m_BoneAnims; }

    private:
        std::string                m_Name{"Clip"};
        float32                    m_Duration{0.0f};
        bool                       m_Looping{true};
        std::vector<BoneAnimation> m_BoneAnims;
    };

} // namespace Good