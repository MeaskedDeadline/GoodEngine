#include "Client.h"
#include "../Core/Log.h"

namespace Good
{

    bool Client::Connect(const std::string& ip, uint16 port)
    {
        m_State = ClientState::Connecting;
        m_Buffer.resize(65536);

        if (!m_Socket.Create(SocketType::TCP))
        {
            m_State = ClientState::Disconnected;
            return false;
        }

        if (!m_Socket.Connect(ip, port))
        {
            m_State = ClientState::Disconnected;
            return false;
        }

        m_Socket.SetNonBlocking(true);
        m_State = ClientState::Connected;
        GOOD_LOG_INFO("Network", "Client connected to {}:{}", ip, port);
        return true;
    }

    void Client::Disconnect()
    {
        if (m_State == ClientState::Disconnected) return;
        m_Socket.Close();
        m_State = ClientState::Disconnected;
        GOOD_LOG_INFO("Network", "Client disconnected");
    }

    bool Client::SendPacket(const Packet& packet)
    {
        if (!IsConnected() || packet.IsEmpty()) return false;

        uint32 size = static_cast<uint32>(packet.GetSize());
        m_Socket.Send(&size, sizeof(size));
        return m_Socket.Send(packet.GetData(), packet.GetSize()) > 0;
    }

    bool Client::ReceivePacket(Packet& outPacket)
    {
        if (!IsConnected()) return false;

        uint32 size = 0;
        int32 received = m_Socket.Receive(&size, sizeof(size));
        if (received <= 0) return false;

        if (size == 0 || size > 65536) return false;

        m_Buffer.resize(size);
        received = m_Socket.Receive(m_Buffer.data(), size);
        if (received <= 0) return false;

        outPacket = Packet();
        outPacket.Write(m_Buffer.data(), static_cast<usize>(received));
        outPacket.Reset();
        return true;
    }

    void Client::Update()
    {
        if (!IsConnected()) return;

        Packet packet;
        while (ReceivePacket(packet))
        {
            if (m_Handler) m_Handler(packet);
        }
    }

} // namespace Good