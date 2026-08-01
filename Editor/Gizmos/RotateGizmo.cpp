#include "Gizmos/RotateGizmo.h"
#include "Math/MathCommon.h"

namespace Good
{

    RotateGizmo::RotateGizmo()
    {
        m_Mode = GizmoMode::Rotate;
    }

    void RotateGizmo::Draw(const Vec3& pos, const Mat4& viewProj)
    {
        float32 r = m_Radius * m_Size;
        int32 segs = m_Segments;

        Color cx = GetAxisColor(GizmoAxis::X, m_ActiveAxis);
        Color cy = GetAxisColor(GizmoAxis::Y, m_ActiveAxis);
        Color cz = GetAxisColor(GizmoAxis::Z, m_ActiveAxis);

        // Двойные линии для толщины
        float32 offset = 0.015f;

        for (int32 i = 0; i < segs; ++i)
        {
            float32 a0 = Math::TWO_PI * static_cast<float32>(i) / segs;
            float32 a1 = Math::TWO_PI * static_cast<float32>(i + 1) / segs;

            float32 c0 = Math::Cos(a0), s0 = Math::Sin(a0);
            float32 c1 = Math::Cos(a1), s1 = Math::Sin(a1);

            // X кольцо (YZ)
            AddLine(pos + Vec3(0, c0*r, s0*r), pos + Vec3(0, c1*r, s1*r), cx);
            AddLine(pos + Vec3(offset, c0*r, s0*r), pos + Vec3(offset, c1*r, s1*r), cx);

            // Y кольцо (XZ)
            AddLine(pos + Vec3(c0*r, 0, s0*r), pos + Vec3(c1*r, 0, s1*r), cy);
            AddLine(pos + Vec3(c0*r, offset, s0*r), pos + Vec3(c1*r, offset, s1*r), cy);

            // Z кольцо (XY)
            AddLine(pos + Vec3(c0*r, s0*r, 0), pos + Vec3(c1*r, s1*r, 0), cz);
            AddLine(pos + Vec3(c0*r, s0*r, offset), pos + Vec3(c1*r, s1*r, offset), cz);
        }

        // Маленькие засечки на 90° для ориентации
        float32 tickLen = 0.15f * m_Size;

        // X засечки
        AddLine(pos + Vec3(0, r, 0), pos + Vec3(0, r + tickLen, 0), cx);
        AddLine(pos + Vec3(0, 0, r), pos + Vec3(0, 0, r + tickLen), cx);

        // Y засечки
        AddLine(pos + Vec3(r, 0, 0), pos + Vec3(r + tickLen, 0, 0), cy);
        AddLine(pos + Vec3(0, 0, r), pos + Vec3(0, 0, r + tickLen), cy);

        // Z засечки
        AddLine(pos + Vec3(r, 0, 0), pos + Vec3(r + tickLen, 0, 0), cz);
        AddLine(pos + Vec3(0, r, 0), pos + Vec3(0, r + tickLen, 0), cz);

        FlushLines(viewProj);
    }

} // namespace Good