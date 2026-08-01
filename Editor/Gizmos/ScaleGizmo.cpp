#include "Gizmos/ScaleGizmo.h"
#include "Math/MathCommon.h"

namespace Good
{

ScaleGizmo::ScaleGizmo()
{
    m_Mode = GizmoMode::Scale;
}

static void DrawWireCube(Gizmo& g, const Vec3& center, float32 size, const Color& color)
{
    float32 h = size;
    Vec3 c[8] = {
        center + Vec3(-h,-h,-h), center + Vec3( h,-h,-h),
        center + Vec3( h, h,-h), center + Vec3(-h, h,-h),
        center + Vec3(-h,-h, h), center + Vec3( h,-h, h),
        center + Vec3( h, h, h), center + Vec3(-h, h, h)
    };
    int32 edges[12][2] = {
        {0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };
    for (auto& e : edges)
        g.AddLine(c[e[0]], c[e[1]], color);
}

void ScaleGizmo::Draw(const Vec3& pos, const Mat4& viewProj)
{
    float32 len = m_Length * m_Size;
    float32 box = m_BoxSize * m_Size;

    Color cx = GetAxisColor(GizmoAxis::X, m_ActiveAxis);
    Color cy = GetAxisColor(GizmoAxis::Y, m_ActiveAxis);
    Color cz = GetAxisColor(GizmoAxis::Z, m_ActiveAxis);
    Color cw(0.85f, 0.85f, 0.85f, 1.0f);

    float32 offset = 0.01f;

    // Оси (двойные для толщины)
    AddLine(pos, pos + Vec3(len, 0, 0), cx);
    AddLine(pos + Vec3(0, offset, 0), pos + Vec3(len, offset, 0), cx);

    AddLine(pos, pos + Vec3(0, len, 0), cy);
    AddLine(pos + Vec3(offset, 0, 0), pos + Vec3(offset, len, 0), cy);

    AddLine(pos, pos + Vec3(0, 0, len), cz);
    AddLine(pos + Vec3(0, offset, 0), pos + Vec3(0, offset, len), cz);

    // Кубики на концах
    DrawWireCube(*this, pos + Vec3(len, 0, 0), box, cx);
    DrawWireCube(*this, pos + Vec3(0, len, 0), box, cy);
    DrawWireCube(*this, pos + Vec3(0, 0, len), box, cz);

    // Центральный кубик (uniform scale)
    DrawWireCube(*this, pos, box * 0.8f, cw);

    // Диагональные линии между осями (показывают связь)
    float32 diagOff = 0.4f * m_Size;
    Color diagColor(0.6f, 0.6f, 0.6f, 0.5f);
    AddLine(pos + Vec3(diagOff, 0, 0), pos + Vec3(0, diagOff, 0), diagColor);
    AddLine(pos + Vec3(diagOff, 0, 0), pos + Vec3(0, 0, diagOff), diagColor);
    AddLine(pos + Vec3(0, diagOff, 0), pos + Vec3(0, 0, diagOff), diagColor);

    FlushLines(viewProj);
}

} // namespace Good