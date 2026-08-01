#pragma once
#include "Light.h"

namespace Good
{

    class DirectionalLight : public Light
    {
    public:
        DirectionalLight();

        void SetDirection(const Vec3& dir);
        Vec3 GetDirection() const;

        void SetAmbient(float32 a) { m_Ambient = a; }
        float32 GetAmbient()  const{ return m_Ambient; }

    private:
        float32 m_Ambient{0.1f};
    };

} // namespace Good