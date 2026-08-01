#pragma once
#include "../Core/Core.h"

namespace Good
{

    class Hash
    {
    public:
        static uint32 FNV1a32(const void* data, usize size);
        static uint64 FNV1a64(const void* data, usize size);

        static uint32 String32(const std::string& s);
        static uint64 String64(const std::string& s);

        static uint32 Combine32(uint32 a, uint32 b);
        static uint64 Combine64(uint64 a, uint64 b);

        template<typename T>
        static uint64 Of(const T& v)
        {
            return FNV1a64(&v, sizeof(T));
        }
    };

} // namespace Good