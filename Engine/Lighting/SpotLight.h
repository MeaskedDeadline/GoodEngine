#pragma once
#include "Light.h"

namespace Good
{

    class SpotLight : public Light
    {
    public:
        SpotLight();

        void SetPosition(const Vec3& pos)   { GetData().position   = pos; }
        void SetDirection(const Vec3& dir)  { GetData().direction  = dir.Normalized(); }
        void SetConeAngle(float32 inner, float32 outer);
        void SetRange(float32 r)            { GetData().range      = r;   }

        float32 GetInnerCone() const { return m_InnerCone; }
        float32 GetOuterCone() const { return GetData().spotAngle; }

    private:
        float32 m_InnerCone{30.0f};
    };

} // namespace Good