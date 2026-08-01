#pragma once
#include "../Core/Core.h"

namespace Good
{

    class UniformBuffer
    {
    public:
        UniformBuffer()  = default;
        ~UniformBuffer();

        void Create(usize sizeBytes, uint32 bindingPoint);
        void Update(const void* data, usize sizeBytes, usize offset = 0);
        void Bind()   const;
        void Unbind() const;

        uint32 GetID() const { return m_ID; }

    private:
        uint32 m_ID{0};
        uint32 m_BindingPoint{0};
    };

} // namespace Good