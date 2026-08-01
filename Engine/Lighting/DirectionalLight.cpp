#include "DirectionalLight.h"
#include "../Math/MathCommon.h"

namespace Good
{

    DirectionalLight::DirectionalLight() : Light(LightType::Directional)
    {
        GetData().direction = Vec3(0,-1,0);
    }

    void DirectionalLight::SetDirection(const Vec3& dir)
    {
        GetData().direction = dir.Normalized();
    }

    Vec3 DirectionalLight::GetDirection() const
    {
        return GetData().direction;
    }

} // namespace Good