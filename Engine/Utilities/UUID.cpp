#include "UUID.h"
#include <random>
#include <sstream>
#include <iomanip>

namespace Good
{

    static std::mt19937_64 s_Generator(std::random_device{}());

    UUID::UUID() : m_Value(0) {}
    UUID::UUID(uint64 v) : m_Value(v) {}

    UUID UUID::Generate()
    {
        return UUID(s_Generator());
    }

    UUID UUID::Invalid()
    {
        return UUID(0);
    }

    std::string UUID::ToString() const
    {
        std::ostringstream oss;
        oss << std::hex << std::setw(16) << std::setfill('0') << m_Value;
        return oss.str();
    }

} // namespace Good