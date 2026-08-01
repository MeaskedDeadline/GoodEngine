#pragma once
#include "../Core/Core.h"
#include "../Math/Vec3.h"

namespace Good
{

    class NavPath
    {
    public:
        NavPath() = default;

        void AddPoint(const Vec3& point);
        void Clear();

        const Vec3& GetPoint(usize index) const;
        usize GetCount()  const { return m_Points.size(); }
        bool  IsEmpty()   const { return m_Points.empty(); }
        bool  IsValid()   const { return !m_Points.empty(); }

        float32 GetTotalLength() const;

    private:
        std::vector<Vec3> m_Points;
    };

} // namespace Good