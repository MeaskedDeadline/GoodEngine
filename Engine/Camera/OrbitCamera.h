#pragma once
#include "CameraController.h"
#include "../Math/Vec3.h"

namespace Good
{

    class OrbitCamera final : public CameraController
    {
    public:
        OrbitCamera();

        void OnUpdate(Camera& camera, float32 dt) override;

        void SetTarget(const Vec3& t) { m_Target = t; }
        void SetDistance(float32 d)   { m_Distance = d; }
        void SetMinDistance(float32 d){ m_MinDist = d; }
        void SetMaxDistance(float32 d){ m_MaxDist = d; }
        void SetSensitivity(float32 s){ m_Sensitivity = s; }

    private:
        Vec3    m_Target{0,0,0};
        float32 m_Distance{10.0f};
        float32 m_MinDist{1.0f};
        float32 m_MaxDist{100.0f};
        float32 m_Yaw{0.0f};
        float32 m_Pitch{20.0f};
        float32 m_Sensitivity{0.3f};
    };

} // namespace Good