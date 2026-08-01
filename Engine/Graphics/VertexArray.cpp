#include "VertexArray.h"
#include "GLFunctions.h"

namespace Good
{

    VertexArray::~VertexArray()
    {
        if (m_ID && GL::DeleteVertexArrays) GL::DeleteVertexArrays(1, &m_ID);
    }

    void VertexArray::Create()
    {
        if (GL::GenVertexArrays) GL::GenVertexArrays(1, &m_ID);
    }

    void VertexArray::Bind()   const { if (GL::BindVertexArray) GL::BindVertexArray(m_ID); }
    void VertexArray::Unbind() const { if (GL::BindVertexArray) GL::BindVertexArray(0);    }

    void VertexArray::SetAttribute(uint32 index, int32 size, uint32 type,
                                    bool normalized, int32 stride, usize offset)
    {
        if (GL::EnableVertexAttribArray) GL::EnableVertexAttribArray(index);
        if (GL::VertexAttribPointer)     GL::VertexAttribPointer(
            index, size, type,
            normalized ? GL_TRUE : GL_FALSE,
            stride,
            reinterpret_cast<const void*>(offset));
    }

} // namespace Good