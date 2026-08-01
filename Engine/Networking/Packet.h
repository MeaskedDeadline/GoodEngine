#pragma once
#include "../Core/Core.h"

namespace Good
{

    enum class PacketType : uint16
    {
        Unknown = 0,
        Ping, Pong,
        Connect, Disconnect,
        PlayerUpdate,
        EntitySync,
        RPCCall,
        ChatMessage,
        Custom = 1000
    };

    class Packet
    {
    public:
        Packet();
        explicit Packet(PacketType type);

        void Write(const void* data, usize size);
        void Read(void* data, usize size);

        template<typename T> void WriteValue(const T& val) { Write(&val, sizeof(T)); }
        template<typename T> T    ReadValue()
        {
            T val{};
            Read(&val, sizeof(T));
            return val;
        }

        void WriteString(const std::string& s);
        std::string ReadString();

        void Reset()   { m_ReadPos = 0; }
        void Clear()   { m_Data.clear(); m_ReadPos = 0; }

        PacketType GetType()     const { return m_Type;             }
        usize      GetSize()     const { return m_Data.size();      }
        const byte* GetData()    const { return m_Data.data();      }
        bool        IsEmpty()    const { return m_Data.empty();     }
        bool        CanRead()    const { return m_ReadPos < m_Data.size(); }

    private:
        PacketType       m_Type{PacketType::Unknown};
        std::vector<byte> m_Data;
        usize            m_ReadPos{0};
    };

} // namespace Good