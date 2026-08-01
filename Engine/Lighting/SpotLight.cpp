#include "SpotLight.h"

namespace Good
{

    SpotLight::SpotLight() : Light(LightType::Spot) {}

    void SpotLight::SetConeAngle(float32 inner, float32 outer)
    {
        m_InnerCone           = inner;
        GetData().spotAngle   = outer;
    }

} // namespace Good