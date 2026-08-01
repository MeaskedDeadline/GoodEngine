#pragma once
#include "../Core/Core.h"

namespace Good
{

    class VertexBuffer
    {
    public:
        VertexBuffer()  = default;
        ~VertexBuffer();

        void Create(const void* data, usize sizeBytes, bool dynamic = false);
        void Update(const void* data, usize sizeBytes, usize offset = 0);
        void Bind()   const;
        void Unbind() const;

        uint32 GetID()   const { return m_ID;   }
        usize  GetSize() const { return m_Size; }

    private:
        uint32 m_ID{0};
        usize  m_Size{0};
    };

} // namespace Good