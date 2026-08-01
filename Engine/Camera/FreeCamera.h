#pragma once
#include "CameraController.h"
#include "../Math/Vec3.h"

namespace Good
{

    class FreeCamera final : public CameraController
    {
    public:
        FreeCamera();

        void OnUpdate(Camera& camera, float32 dt) override;

        void SetSpeed(float32 s)        { m_Speed = s;    }
        void SetSensitivity(float32 s)  { m_Sensitivity = s; }
        void SetSprintMultiplier(float32 m){ m_SprintMult = m; }

    private:
        float32 m_Speed{5.0f};
        float32 m_Sensitivity{0.2f};
        float32 m_SprintMult{3.0f};
        float32 m_Yaw{0.0f};
        float32 m_Pitch{0.0f};
        bool    m_FirstMouse{true};
    };

} // namespace Good