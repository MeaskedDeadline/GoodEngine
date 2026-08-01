#pragma once
#include "../Core/Core.h"

namespace Good
{

    class IndexBuffer
    {
    public:
        IndexBuffer()  = default;
        ~IndexBuffer();

        void Create(const uint32* data, uint32 count);
        void Bind()   const;
        void Unbind() const;

        uint32 GetID()    const { return m_ID;    }
        uint32 GetCount() const { return m_Count; }

    private:
        uint32 m_ID{0};
        uint32 m_Count{0};
    };

} // namespace Good