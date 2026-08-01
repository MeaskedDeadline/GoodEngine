#include "PointLight.h"

namespace Good
{

    PointLight::PointLight() : Light(LightType::Point) {}

    void PointLight::SetAttenuation(float32 c, float32 l, float32 q)
    {
        m_ConstantAtt  = c;
        m_LinearAtt    = l;
        m_QuadraticAtt = q;
    }

} // namespace Good