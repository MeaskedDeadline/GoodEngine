#include "ShadowMap.h"
#include "../Graphics/GLFunctions.h"
#include "../Core/Log.h"

namespace Good
{

    bool ShadowMap::Create(uint32 resolution)
    {
        m_Resolution = resolution;

        glGenTextures(1, &m_DepthTexture);
        glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
            static_cast<GLsizei>(resolution), static_cast<GLsizei>(resolution),
            0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if (!GL::GenFramebuffers) { GOOD_LOG_WARN("ShadowMap", "FBO not available"); return false; }

        GL::GenFramebuffers(1, &m_FBO);
        GL::BindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        GL::FramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

        GL::BindFramebuffer(GL_FRAMEBUFFER, 0);
        GOOD_LOG_INFO("ShadowMap", "Created {}x{}", resolution, resolution);
        return true;
    }

    void ShadowMap::Destroy()
    {
        if (m_DepthTexture) { glDeleteTextures(1, &m_DepthTexture); m_DepthTexture = 0; }
        if (m_FBO && GL::DeleteFramebuffers) { GL::DeleteFramebuffers(1, &m_FBO); m_FBO = 0; }
    }

    void ShadowMap::BindForWriting()
    {
        if (GL::BindFramebuffer)
        {
            GL::BindFramebuffer(GL_FRAMEBUFFER, m_FBO);
            glViewport(0, 0, static_cast<GLsizei>(m_Resolution), static_cast<GLsizei>(m_Resolution));
            glClear(GL_DEPTH_BUFFER_BIT);
        }
    }

    void ShadowMap::UnbindForWriting()
    {
        if (GL::BindFramebuffer) GL::BindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void ShadowMap::BindForReading(uint32 slot) const
    {
        if (GL::ActiveTexture) GL::ActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
    }

} // namespace Good