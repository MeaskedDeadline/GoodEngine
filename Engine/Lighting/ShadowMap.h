#pragma once
#include "../Core/Core.h"
#include "../Math/Mat4.h"
#include "../Graphics/Framebuffer.h"

namespace Good
{

    class ShadowMap
    {
    public:
        ShadowMap()  = default;
        ~ShadowMap() = default;

        bool Create(uint32 resolution = 1024);
        void Destroy();

        void BindForWriting();
        void UnbindForWriting();
        void BindForReading(uint32 slot = 0) const;

        const Mat4& GetLightMatrix() const { return m_LightMatrix; }
        void SetLightMatrix(const Mat4& m)  { m_LightMatrix = m; }

        uint32 GetResolution() const { return m_Resolution; }
        uint32 GetTextureID()  const { return m_DepthTexture; }

    private:
        uint32 m_FBO{0};
        uint32 m_DepthTexture{0};
        uint32 m_Resolution{1024};
        Mat4   m_LightMatrix;
    };

} // namespace Good