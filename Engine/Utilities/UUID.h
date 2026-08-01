#pragma once
#include "../Core/Core.h"

namespace Good
{

    class UUID
    {
    public:
        UUID();
        explicit UUID(uint64 value);

        uint64 GetValue() const { return m_Value; }

        bool operator==(const UUID& o) const { return m_Value == o.m_Value; }
        bool operator!=(const UUID& o) const { return m_Value != o.m_Value; }
        bool operator< (const UUID& o) const { return m_Value <  o.m_Value; }

        std::string ToString() const;
        bool        IsValid()  const { return m_Value != 0; }

        static UUID Generate();
        static UUID Invalid();

        struct Hash
        {
            usize operator()(const UUID& u) const
            {
                return std::hash<uint64>()(u.m_Value);
            }
        };

    private:
        uint64 m_Value{0};
    };

} // namespace Good