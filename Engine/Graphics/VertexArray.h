#pragma once
#include "../Core/Core.h"

namespace Good
{

    class VertexArray
    {
    public:
        VertexArray()  = default;
        ~VertexArray();

        void Create();
        void Bind()   const;
        void Unbind() const;

        void SetAttribute(uint32 index, int32 size, uint32 type,
                          bool normalized, int32 stride, usize offset);

        uint32 GetID() const { return m_ID; }

    private:
        uint32 m_ID{0};
    };

} // namespace Good