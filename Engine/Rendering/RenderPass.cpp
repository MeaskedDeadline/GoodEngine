#include "RenderPass.h"
#include "../Graphics/GLFunctions.h"

namespace Good
{

    void RenderPass::Begin()
    {
        if (m_Framebuffer) m_Framebuffer->Bind();
    }

    void RenderPass::End()
    {
        if (m_Framebuffer) m_Framebuffer->Unbind();
    }

} // namespace Good