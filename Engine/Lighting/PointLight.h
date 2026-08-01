#pragma once
#include "Light.h"

namespace Good
{

    class PointLight : public Light
    {
    public:
        PointLight();

        void SetPosition(const Vec3& pos) { GetData().position = pos; }
        void SetRange(float32 range)      { GetData().range = range;  }
        void SetAttenuation(float32 constant, float32 linear, float32 quadratic);

        float32 GetConstantAtt()  const { return m_ConstantAtt;  }
        float32 GetLinearAtt()    const { return m_LinearAtt;    }
        float32 GetQuadraticAtt() const { return m_QuadraticAtt; }

    private:
        float32 m_ConstantAtt{1.0f};
        float32 m_LinearAtt{0.09f};
        float32 m_QuadraticAtt{0.032f};
    };

} // namespace Good