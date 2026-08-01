#include "Gizmos/TranslateGizmo.h"
#include "Math/MathCommon.h"

namespace Good
{

TranslateGizmo::TranslateGizmo()
{
    m_Mode = GizmoMode::Translate;
}

static void DrawCone(Gizmo& g, const Vec3& tip, const Vec3& dir,
                     float32 height, float32 radius, const Color& color, int32 segs = 8)
{
    Vec3 base = tip - dir * height;

    // Найдём перпендикулярные векторы
    Vec3 perp1, perp2;
    if (Math::Abs(dir.y) < 0.99f)
        perp1 = dir.Cross(Vec3(0, 1, 0)).Normalized();
    else
        perp1 = dir.Cross(Vec3(1, 0, 0)).Normalized();
    perp2 = dir.Cross(perp1).Normalized();

    for (int32 i = 0; i < segs; ++i)
    {
        float32 a0 = Math::TWO_PI * static_cast<float32>(i) / segs;
        float32 a1 = Math::TWO_PI * static_cast<float32>(i + 1) / segs;

        Vec3 p0 = base + (perp1 * Math::Cos(a0) + perp2 * Math::Sin(a0)) * radius;
        Vec3 p1 = base + (perp1 * Math::Cos(a1) + perp2 * Math::Sin(a1)) * radius;

        // Стороны конуса
        g.AddLine(tip, p0, color);
        g.AddLine(tip, p1, color);

        // Основание
        g.AddLine(p0, p1, color);
    }
}

static void DrawPlaneQuad(Gizmo& g, const Vec3& pos,
                          const Vec3& axis1, const Vec3& axis2,
                          float32 offset, float32 size, const Color& color)
{
    Vec3 corner = pos + axis1 * offset + axis2 * offset;
    Vec3 a = corner;
    Vec3 b = corner + axis1 * size;
    Vec3 c = corner + axis1 * size + axis2 * size;
    Vec3 d = corner + axis2 * size;

    g.AddLine(a, b, color);
    g.AddLine(b, c, color);
    g.AddLine(c, d, color);
    g.AddLine(d, a, color);

    // Диагональ для заметности
    g.AddLine(a, c, color);
}

void TranslateGizmo::Draw(const Vec3& pos, const Mat4& viewProj)
{
    float32 len = m_Length * m_Size;
    float32 coneH = 0.35f * m_Size;
    float32 coneR = 0.1f * m_Size;
    float32 quadOff = 0.3f * m_Size;
    float32 quadSize = 0.25f * m_Size;

    Color cx = GetAxisColor(GizmoAxis::X, m_ActiveAxis);
    Color cy = GetAxisColor(GizmoAxis::Y, m_ActiveAxis);
    Color cz = GetAxisColor(GizmoAxis::Z, m_ActiveAxis);

    // ═══ Оси (толстые — двойные линии) ═══

    // X ось
    AddLine(pos, pos + Vec3(len, 0, 0), cx);
    AddLine(pos + Vec3(0, 0.01f, 0), pos + Vec3(len, 0.01f, 0), cx);

    // Y ось
    AddLine(pos, pos + Vec3(0, len, 0), cy);
    AddLine(pos + Vec3(0.01f, 0, 0), pos + Vec3(0.01f, len, 0), cy);

    // Z ось
    AddLine(pos, pos + Vec3(0, 0, len), cz);
    AddLine(pos + Vec3(0, 0.01f, 0), pos + Vec3(0, 0.01f, len), cz);

    // ═══ Конусы на концах осей ═══
    DrawCone(*this, pos + Vec3(len, 0, 0), Vec3(1, 0, 0), coneH, coneR, cx);
    DrawCone(*this, pos + Vec3(0, len, 0), Vec3(0, 1, 0), coneH, coneR, cy);
    DrawCone(*this, pos + Vec3(0, 0, len), Vec3(0, 0, 1), coneH, coneR, cz);

    // ═══ Квадратики для 2-осевого движения ═══
    Color xyColor(0.9f, 0.9f, 0.2f, 0.8f);
    Color xzColor(0.9f, 0.2f, 0.9f, 0.8f);
    Color yzColor(0.2f, 0.9f, 0.9f, 0.8f);

    DrawPlaneQuad(*this, pos, Vec3(1,0,0), Vec3(0,1,0), quadOff, quadSize, xyColor);
    DrawPlaneQuad(*this, pos, Vec3(1,0,0), Vec3(0,0,1), quadOff, quadSize, xzColor);
    DrawPlaneQuad(*this, pos, Vec3(0,1,0), Vec3(0,0,1), quadOff, quadSize, yzColor);

    // ═══ Центральный кубик ═══
    float32 cs = 0.08f * m_Size;
    Color cw(0.9f, 0.9f, 0.9f, 1.0f);
    AddLine(pos + Vec3(-cs,-cs,-cs), pos + Vec3( cs,-cs,-cs), cw);
    AddLine(pos + Vec3( cs,-cs,-cs), pos + Vec3( cs, cs,-cs), cw);
    AddLine(pos + Vec3( cs, cs,-cs), pos + Vec3(-cs, cs,-cs), cw);
    AddLine(pos + Vec3(-cs, cs,-cs), pos + Vec3(-cs,-cs,-cs), cw);
    AddLine(pos + Vec3(-cs,-cs, cs), pos + Vec3( cs,-cs, cs), cw);
    AddLine(pos + Vec3( cs,-cs, cs), pos + Vec3( cs, cs, cs), cw);
    AddLine(pos + Vec3( cs, cs, cs), pos + Vec3(-cs, cs, cs), cw);
    AddLine(pos + Vec3(-cs, cs, cs), pos + Vec3(-cs,-cs, cs), cw);
    AddLine(pos + Vec3(-cs,-cs,-cs), pos + Vec3(-cs,-cs, cs), cw);
    AddLine(pos + Vec3( cs,-cs,-cs), pos + Vec3( cs,-cs, cs), cw);
    AddLine(pos + Vec3( cs, cs,-cs), pos + Vec3( cs, cs, cs), cw);
    AddLine(pos + Vec3(-cs, cs,-cs), pos + Vec3(-cs, cs, cs), cw);

    FlushLines(viewProj);
}

} // namespace Good