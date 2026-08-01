#include "VertexBuffer.h"
#include "GLFunctions.h"

namespace Good
{

    VertexBuffer::~VertexBuffer()
    {
        if (m_ID && GL::DeleteBuffers) GL::DeleteBuffers(1, &m_ID);
    }

    void VertexBuffer::Create(const void* data, usize sizeBytes, bool dynamic)
    {
        if (GL::GenBuffers)  GL::GenBuffers(1, &m_ID);
        if (GL::BindBuffer)  GL::BindBuffer(GL_ARRAY_BUFFER, m_ID);
        if (GL::BufferData)  GL::BufferData(GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeBytes), data,
            dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        m_Size = sizeBytes;
    }

    void VertexBuffer::Update(const void* data, usize sizeBytes, usize offset)
    {
        if (GL::BindBuffer)   GL::BindBuffer(GL_ARRAY_BUFFER, m_ID);
        if (GL::BufferSubData) GL::BufferSubData(GL_ARRAY_BUFFER,
            static_cast<GLintptr>(offset),
            static_cast<GLsizeiptr>(sizeBytes), data);
    }

    void VertexBuffer::Bind()   const { if (GL::BindBuffer) GL::BindBuffer(GL_ARRAY_BUFFER, m_ID); }
    void VertexBuffer::Unbind() const { if (GL::BindBuffer) GL::BindBuffer(GL_ARRAY_BUFFER, 0);    }

} // namespace Good