#include "NavPath.h"

namespace Good
{

    void NavPath::AddPoint(const Vec3& point) { m_Points.push_back(point); }
    void NavPath::Clear()                     { m_Points.clear(); }

    const Vec3& NavPath::GetPoint(usize index) const
    {
        static Vec3 s_Zero;
        return index < m_Points.size() ? m_Points[index] : s_Zero;
    }

    float32 NavPath::GetTotalLength() const
    {
        float32 total = 0.0f;
        for (usize i = 1; i < m_Points.size(); ++i)
            total += (m_Points[i] - m_Points[i-1]).Length();
        return total;
    }

} // namespace Good