#include "IndexBuffer.h"
#include "GLFunctions.h"

namespace Good
{

    IndexBuffer::~IndexBuffer()
    {
        if (m_ID && GL::DeleteBuffers) GL::DeleteBuffers(1, &m_ID);
    }

    void IndexBuffer::Create(const uint32* data, uint32 count)
    {
        m_Count = count;
        if (GL::GenBuffers)  GL::GenBuffers(1, &m_ID);
        if (GL::BindBuffer)  GL::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
        if (GL::BufferData)  GL::BufferData(GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(count * sizeof(uint32)), data, GL_STATIC_DRAW);
    }

    void IndexBuffer::Bind()   const { if (GL::BindBuffer) GL::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID); }
    void IndexBuffer::Unbind() const { if (GL::BindBuffer) GL::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);    }

} // namespace Good