#pragma once
#include "../Core/Core.h"
#include "Socket.h"
#include "Packet.h"
#include <functional>

namespace Good
{

    enum class ClientState : uint8 { Disconnected, Connecting, Connected };

    using PacketHandler = std::function<void(const Packet&)>;

    class Client : public NonCopyable
    {
    public:
        Client() = default;
        ~Client() { Disconnect(); }

        bool Connect(const std::string& ip, uint16 port);
        void Disconnect();

        bool SendPacket(const Packet& packet);
        bool ReceivePacket(Packet& outPacket);

        void Update();

        void SetPacketHandler(PacketHandler handler) { m_Handler = std::move(handler); }

        ClientState GetState()   const { return m_State;    }
        bool        IsConnected()const { return m_State == ClientState::Connected; }

    private:
        Socket      m_Socket;
        ClientState m_State{ClientState::Disconnected};
        PacketHandler m_Handler;
        std::vector<byte> m_Buffer;
    };

} // namespace Good