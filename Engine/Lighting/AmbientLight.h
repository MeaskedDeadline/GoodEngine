#pragma once
#include "Light.h"

namespace Good
{

    class AmbientLight : public Light
    {
    public:
        AmbientLight();

        void SetSkyColor(const Color& sky)     { m_SkyColor = sky;     }
        void SetGroundColor(const Color& gnd)  { m_GroundColor = gnd;  }

        const Color& GetSkyColor()    const { return m_SkyColor;    }
        const Color& GetGroundColor() const { return m_GroundColor; }

    private:
        Color m_SkyColor{0.2f, 0.2f, 0.3f, 1.0f};
        Color m_GroundColor{0.1f, 0.1f, 0.1f, 1.0f};
    };

} // namespace Good