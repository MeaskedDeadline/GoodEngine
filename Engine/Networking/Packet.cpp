#include "Packet.h"
#include <cstring>

namespace Good
{

    Packet::Packet()                : m_Type(PacketType::Unknown) {}
    Packet::Packet(PacketType type) : m_Type(type) {}

    void Packet::Write(const void* data, usize size)
    {
        const byte* bytes = static_cast<const byte*>(data);
        m_Data.insert(m_Data.end(), bytes, bytes + size);
    }

    void Packet::Read(void* data, usize size)
    {
        if (m_ReadPos + size > m_Data.size()) return;
        std::memcpy(data, m_Data.data() + m_ReadPos, size);
        m_ReadPos += size;
    }

    void Packet::WriteString(const std::string& s)
    {
        uint32 len = static_cast<uint32>(s.size());
        WriteValue(len);
        if (len > 0) Write(s.data(), len);
    }

    std::string Packet::ReadString()
    {
        uint32 len = ReadValue<uint32>();
        if (len == 0 || m_ReadPos + len > m_Data.size()) return "";
        std::string s(reinterpret_cast<const char*>(m_Data.data() + m_ReadPos), len);
        m_ReadPos += len;
        return s;
    }

} // namespace Good