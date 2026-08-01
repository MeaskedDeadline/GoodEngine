#pragma once
#include "../Core/Core.h"
#include "../Math/Vec3.h"
#include "../Math/Mat4.h"
#include "../Math/Quat.h"
#include "../Math/Transform.h"

namespace Good
{

    enum class CameraMode : uint8 { Perspective, Orthographic };

    struct CameraConfig
    {
        CameraMode mode{CameraMode::Perspective};
        float32    fovDegrees{60.0f};
        float32    nearPlane{0.1f};
        float32    farPlane{1000.0f};
        float32    orthoSize{10.0f};
        float32    aspectRatio{16.0f/9.0f};
    };

    class Camera
    {
    public:
        Camera();
        ~Camera() = default;

        void SetConfig(const CameraConfig& cfg) { m_Config = cfg; }
        const CameraConfig& GetConfig()   const { return m_Config; }

        void SetAspect(float32 a)  { m_Config.aspectRatio = a; }
        void SetFOV(float32 fov)   { m_Config.fovDegrees  = fov; }
        void SetNear(float32 n)    { m_Config.nearPlane   = n; }
        void SetFar(float32 f)     { m_Config.farPlane    = f; }

        Transform&       GetTransform()       { return m_Transform; }
        const Transform& GetTransform() const { return m_Transform; }

        Mat4 GetViewMatrix()       const;
        Mat4 GetProjectionMatrix() const;
        Mat4 GetViewProjection()   const;

        Vec3 ScreenToWorldDirection(float32 ndcX, float32 ndcY) const;

    private:
        Transform    m_Transform;
        CameraConfig m_Config;
    };

} // namespace Good