#include "Hash.h"

namespace Good
{

    uint32 Hash::FNV1a32(const void* data, usize size)
    {
        uint32 hash = 0x811c9dc5u;
        const byte* p = static_cast<const byte*>(data);
        for (usize i = 0; i < size; ++i)
        {
            hash ^= static_cast<uint32>(p[i]);
            hash *= 0x01000193u;
        }
        return hash;
    }

    uint64 Hash::FNV1a64(const void* data, usize size)
    {
        uint64 hash = 0xcbf29ce484222325ull;
        const byte* p = static_cast<const byte*>(data);
        for (usize i = 0; i < size; ++i)
        {
            hash ^= static_cast<uint64>(p[i]);
            hash *= 0x100000001b3ull;
        }
        return hash;
    }

    uint32 Hash::String32(const std::string& s) { return FNV1a32(s.data(), s.size()); }
    uint64 Hash::String64(const std::string& s) { return FNV1a64(s.data(), s.size()); }

    uint32 Hash::Combine32(uint32 a, uint32 b)
    {
        return a ^ (b + 0x9e3779b9u + (a << 6) + (a >> 2));
    }

    uint64 Hash::Combine64(uint64 a, uint64 b)
    {
        return a ^ (b + 0x9e3779b97f4a7c15ull + (a << 6) + (a >> 2));
    }

} // namespace Good