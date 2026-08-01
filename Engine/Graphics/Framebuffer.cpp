#include "Framebuffer.h"
#include "GLFunctions.h"
#include "../Core/Log.h"

namespace Good
{

Framebuffer::~Framebuffer() { Destroy(); }

bool Framebuffer::Create(const FramebufferConfig& config)
{
    m_Config = config;
    return BuildAttachments();
}

void Framebuffer::Destroy()
{
    if (!m_ColorTextures.empty())
    {
        glDeleteTextures(static_cast<GLsizei>(m_ColorTextures.size()),
                         m_ColorTextures.data());
        m_ColorTextures.clear();
    }
    if (m_DepthRBO && GL::DeleteRenderbuffers)
        GL::DeleteRenderbuffers(1, &m_DepthRBO);
    if (m_ID && GL::DeleteFramebuffers)
        GL::DeleteFramebuffers(1, &m_ID);
    m_ID = m_DepthRBO = 0;
}

void Framebuffer::Resize(uint32 w, uint32 h)
{
    m_Config.width  = w;
    m_Config.height = h;
    Destroy();
    BuildAttachments();
}

void Framebuffer::Bind()   const { if (GL::BindFramebuffer) GL::BindFramebuffer(GL_FRAMEBUFFER, m_ID); }
void Framebuffer::Unbind() const { if (GL::BindFramebuffer) GL::BindFramebuffer(GL_FRAMEBUFFER, 0);    }

uint32 Framebuffer::GetColorAttachment(uint32 i) const
{
    return i < m_ColorTextures.size() ? m_ColorTextures[i] : 0;
}

uint32 Framebuffer::GetDepthAttachment() const { return m_DepthRBO; }

bool Framebuffer::BuildAttachments()
{
    if (!GL::GenFramebuffers) { GOOD_LOG_WARN("Framebuffer", "Framebuffers not available"); return false; }

    GL::GenFramebuffers(1, &m_ID);
    GL::BindFramebuffer(GL_FRAMEBUFFER, m_ID);

    // Color attachments
    m_ColorTextures.resize(m_Config.colorAttachments);
    glGenTextures(static_cast<GLsizei>(m_Config.colorAttachments), m_ColorTextures.data());

    for (uint32 i = 0; i < m_Config.colorAttachments; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, m_ColorTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
            static_cast<GLsizei>(m_Config.width),
            static_cast<GLsizei>(m_Config.height),
            0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GL::FramebufferTexture2D(GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_ColorTextures[i], 0);
    }

    // Depth + stencil
    if (m_Config.hasDepth)
    {
        GL::GenRenderbuffers(1, &m_DepthRBO);
        GL::BindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
        GL::RenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
            static_cast<GLsizei>(m_Config.width),
            static_cast<GLsizei>(m_Config.height));
        GL::FramebufferRenderbuffer(GL_FRAMEBUFFER,
            GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthRBO);
    }

    bool ok = (GL::CheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    GL::BindFramebuffer(GL_FRAMEBUFFER, 0);

    if (!ok) { GOOD_LOG_ERROR("Framebuffer", "Incomplete framebuffer!"); Destroy(); return false; }
    GOOD_LOG_INFO("Framebuffer", "Created {}x{}", m_Config.width, m_Config.height);
    return true;
}

} // namespace Good