#pragma once
#include "Gizmo.h"

namespace Good
{

    class RotateGizmo final : public Gizmo
    {
    public:
        RotateGizmo();
        void Draw(const Vec3& position, const Mat4& viewProj) override;

    private:
        float32 m_Radius{1.5f};
        int32   m_Segments{32};
    };

} // namespace Good