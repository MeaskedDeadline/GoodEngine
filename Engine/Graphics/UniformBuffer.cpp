#include "UniformBuffer.h"
#include "GLFunctions.h"

// GL_UNIFORM_BUFFER_BINDING — для UBO нужен glBindBufferBase
// Используем расширения через GLFunctions

#ifndef GL_UNIFORM_BUFFER
    #define GL_UNIFORM_BUFFER 0x8A11
#endif

typedef void (APIENTRY* PFNGLBINDBUFFERBASEPROC)(GLenum, GLuint, GLuint);
static PFNGLBINDBUFFERBASEPROC s_glBindBufferBase = nullptr;

namespace Good
{

    UniformBuffer::~UniformBuffer()
    {
        if (m_ID && GL::DeleteBuffers) GL::DeleteBuffers(1, &m_ID);
    }

    void UniformBuffer::Create(usize sizeBytes, uint32 bindingPoint)
    {
        m_BindingPoint = bindingPoint;

        if (GL::GenBuffers)  GL::GenBuffers(1, &m_ID);
        if (GL::BindBuffer)  GL::BindBuffer(GL_UNIFORM_BUFFER, m_ID);
        if (GL::BufferData)  GL::BufferData(GL_UNIFORM_BUFFER,
            static_cast<GLsizeiptr>(sizeBytes), nullptr, GL_DYNAMIC_DRAW);

        if (!s_glBindBufferBase)
        {
#if defined(GOOD_PLATFORM_WINDOWS)
            PROC p = wglGetProcAddress("glBindBufferBase");
            if (p) { union { PROC src; PFNGLBINDBUFFERBASEPROC dst; } u; u.src = p; s_glBindBufferBase = u.dst; }
#endif
        }

        if (s_glBindBufferBase)
            s_glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_ID);
    }

    void UniformBuffer::Update(const void* data, usize sizeBytes, usize offset)
    {
        if (GL::BindBuffer)   GL::BindBuffer(GL_UNIFORM_BUFFER, m_ID);
        if (GL::BufferSubData) GL::BufferSubData(GL_UNIFORM_BUFFER,
            static_cast<GLintptr>(offset),
            static_cast<GLsizeiptr>(sizeBytes), data);
    }

    void UniformBuffer::Bind()   const { if (GL::BindBuffer) GL::BindBuffer(GL_UNIFORM_BUFFER, m_ID); }
    void UniformBuffer::Unbind() const { if (GL::BindBuffer) GL::BindBuffer(GL_UNIFORM_BUFFER, 0);    }

} // namespace Good