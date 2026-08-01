#pragma once
#include "Core/Core.h"
#include "Math/Vec3.h"
#include "Math/Mat4.h"
#include "Math/Color.h"
#include "Physics/Ray.h"

namespace Good
{

    enum class GizmoMode : uint8
    {
        Translate = 0,
        Rotate,
        Scale,
        None
    };

    enum class GizmoAxis : uint8
    {
        None = 0,
        X, Y, Z
    };

    enum class GizmoSpace : uint8
    {
        World = 0,
        Local
    };

    class Gizmo
    {
    public:
        virtual ~Gizmo() = default;

        virtual void Draw(const Vec3& position, const Mat4& viewProj) = 0;

        virtual GizmoAxis HitTest(const Vec3& position, const Ray& ray,
                                   float32 threshold = 0.4f) const;

        GizmoMode  GetMode()       const { return m_Mode;       }
        GizmoSpace GetSpace()      const { return m_Space;      }
        GizmoAxis  GetActiveAxis() const { return m_ActiveAxis; }

        void SetMode(GizmoMode m)       { m_Mode       = m; }
        void SetSpace(GizmoSpace s)     { m_Space      = s; }
        void SetSize(float32 s)         { m_Size       = s; }
        void SetActiveAxis(GizmoAxis a) { m_ActiveAxis = a; }

        static Vec3  GetAxisDirection(GizmoAxis axis);
        static Color GetAxisColor(GizmoAxis axis, GizmoAxis active);

        // Публичный — используется в DrawCone, DrawWireCube и т.д.
        void AddLine(const Vec3& from, const Vec3& to, const Color& color);

    protected:
        void FlushLines(const Mat4& viewProj);
        void InitGL();

        GizmoMode  m_Mode{GizmoMode::Translate};
        GizmoSpace m_Space{GizmoSpace::World};
        GizmoAxis  m_ActiveAxis{GizmoAxis::None};
        float32    m_Size{1.0f};

        struct GizmoLine
        {
            Vec3  from, to;
            Color color;
        };

        std::vector<GizmoLine> m_Lines;

        static uint32 s_VAO;
        static uint32 s_VBO;
        static uint32 s_Shader;
        static bool   s_Inited;
    };

} // namespace Good