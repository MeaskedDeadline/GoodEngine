#pragma once
#include "Gizmo.h"

namespace Good
{

    class ScaleGizmo final : public Gizmo
    {
    public:
        ScaleGizmo();
        void Draw(const Vec3& position, const Mat4& viewProj) override;

    private:
        float32 m_Length{2.0f};
        float32 m_BoxSize{0.15f};
    };

} // namespace Good