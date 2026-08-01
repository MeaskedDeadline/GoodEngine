#pragma once
#include "Component.h"
#include "../Math/Color.h"
#include "../Math/Vec3.h"

namespace Good
{

    enum class LightType : uint8 { Directional, Point, Spot, Ambient };

    class LightComponent final : public Component
    {
    public:
        const char* GetTypeName() const override { return "LightComponent"; }

        void SetType(LightType t)        { m_Type = t;        }
        void SetColor(const Color& c)    { m_Color = c;       }
        void SetIntensity(float32 i)     { m_Intensity = i;   }
        void SetRange(float32 r)         { m_Range = r;       }
        void SetSpotAngle(float32 a)     { m_SpotAngle = a;   }
        void SetCastShadows(bool v)      { m_CastShadows = v; }

        LightType    GetLightType()    const { return m_Type;        }
        const Color& GetColor()        const { return m_Color;       }
        float32      GetIntensity()    const { return m_Intensity;   }
        float32      GetRange()        const { return m_Range;       }
        float32      GetSpotAngle()    const { return m_SpotAngle;   }
        bool         GetCastShadows()  const { return m_CastShadows; }

    private:
        LightType m_Type{LightType::Point};
        Color     m_Color{1,1,1,1};
        float32   m_Intensity{1.0f};
        float32   m_Range{10.0f};
        float32   m_SpotAngle{45.0f};
        bool      m_CastShadows{true};
    };

} // namespace Good