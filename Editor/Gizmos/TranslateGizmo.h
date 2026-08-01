#pragma once
#include "Gizmo.h"

namespace Good
{

    class TranslateGizmo final : public Gizmo
    {
    public:
        TranslateGizmo();
        void Draw(const Vec3& position, const Mat4& viewProj) override;

    private:
        float32 m_Length{2.0f};
        float32 m_ArrowSize{0.25f};
    };

} // namespace Good