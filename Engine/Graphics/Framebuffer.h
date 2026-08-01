#pragma once
#include "../Core/Core.h"

namespace Good
{

    struct FramebufferConfig
    {
        uint32 width{1280};
        uint32 height{720};
        bool   hasDepth{true};
        bool   hasStencil{true};
        uint32 colorAttachments{1};
    };

    class Framebuffer
    {
    public:
        Framebuffer()  = default;
        ~Framebuffer();

        bool Create(const FramebufferConfig& config);
        void Destroy();
        void Resize(uint32 width, uint32 height);

        void Bind()   const;
        void Unbind() const;

        uint32 GetColorAttachment(uint32 index = 0) const;
        uint32 GetDepthAttachment() const;
        uint32 GetWidth()  const { return m_Config.width;  }
        uint32 GetHeight() const { return m_Config.height; }
        bool   IsValid()   const { return m_ID != 0;       }

    private:
        bool BuildAttachments();

        uint32              m_ID{0};
        uint32              m_DepthRBO{0};
        std::vector<uint32> m_ColorTextures;
        FramebufferConfig   m_Config;
    };

} // namespace Good