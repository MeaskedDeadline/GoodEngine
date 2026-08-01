#pragma once
#include "Core/Core.h"
#include "Math/Vec3.h"
#include "Math/Quat.h"

namespace Good
{

template<typename T>
struct Keyframe
{
    float32 time{0.0f};
    T       value;

    Keyframe() = default;
    Keyframe(float32 t, const T& v) : time(t), value(v) {}
};

template<typename T>
class AnimationTrack
{
public:
    void AddKeyframe(float32 time, const T& value)
    {
        m_Keyframes.push_back({time, value});
    }

    T Evaluate(float32 time) const
    {
        if (m_Keyframes.empty()) return T{};
        if (m_Keyframes.size() == 1 || time <= m_Keyframes.front().time)
            return m_Keyframes.front().value;
        if (time >= m_Keyframes.back().time)
            return m_Keyframes.back().value;

        for (usize i = 0; i < m_Keyframes.size()-1; ++i)
        {
            if (time <= m_Keyframes[i+1].time)
            {
                float32 t = (time - m_Keyframes[i].time) /
                            (m_Keyframes[i+1].time - m_Keyframes[i].time);
                return Interpolate(m_Keyframes[i].value, m_Keyframes[i+1].value, t);
            }
        }

        return m_Keyframes.back().value;
    }

    bool    IsEmpty()    const { return m_Keyframes.empty(); }
    float32 GetDuration()const
    {
        return m_Keyframes.empty() ? 0.0f : m_Keyframes.back().time;
    }

private:
    T Interpolate(const T& a, const T& b, float32 t) const;
    std::vector<Keyframe<T>> m_Keyframes;
};

template<>
inline Vec3 AnimationTrack<Vec3>::Interpolate(const Vec3& a, const Vec3& b, float32 t) const
{
    return a.Lerp(b, t);
}

template<>
inline Quat AnimationTrack<Quat>::Interpolate(const Quat& a, const Quat& b, float32 t) const
{
    return Quat::Slerp(a, b, t);
}

template<>
inline float32 AnimationTrack<float32>::Interpolate(const float32& a, const float32& b, float32 t) const
{
    return a + (b - a) * t;
}

using Vec3Track  = AnimationTrack<Vec3>;
using QuatTrack  = AnimationTrack<Quat>;
using FloatTrack = AnimationTrack<float32>;

} // namespace Good