#pragma once
#include "../Core/Core.h"
#include "../Math/Vec3.h"
#include "../Math/Color.h"

namespace Good
{

    enum class LightType : uint8 { Directional, Point, Spot, Ambient };

    struct LightData
    {
        Vec3      position{0,0,0};
        Vec3      direction{0,-1,0};
        Color     color{1,1,1,1};
        float32   intensity{1.0f};
        float32   range{10.0f};
        float32   spotAngle{45.0f};
        float32   spotSoftAngle{5.0f};
        LightType type{LightType::Point};
        bool      castShadows{true};
    };

    class Light
    {
    public:
        Light() = default;
        explicit Light(LightType type);

        void SetData(const LightData& data) { m_Data = data; }
        const LightData& GetData()    const { return m_Data; }
        LightData&       GetData()          { return m_Data; }

        bool IsEnabled() const { return m_Enabled; }
        void SetEnabled(bool v){ m_Enabled = v; }

        LightType GetLightType() const { return m_Data.type; }

    private:
        LightData m_Data;
        bool      m_Enabled{true};
    };

} // namespace Good