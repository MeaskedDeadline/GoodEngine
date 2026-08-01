#pragma once
#include "../Core/Core.h"
#include "Camera.h"

namespace Good
{

    class CameraController
    {
    public:
        virtual ~CameraController() = default;
        virtual void OnUpdate(Camera& camera, float32 dt) = 0;
    };

} // namespace Good