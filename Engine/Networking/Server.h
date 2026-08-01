#pragma once
#include "../Core/Core.h"
#include "Socket.h"
#include "Packet.h"
#include <functional>

namespace Good
{

    struct ClientConnection
    {
        uint32 id{0};
        Socket socket;
        std::string ip;
        uint16 port{0};
        bool connected{true};
    };

    using ServerPacketHandler = std::function<void(uint32 clientID, const Packet&)>;
    using ClientConnectHandler = std::function<void(uint32 clientID)>;
    using ClientDisconnectHandler = std::function<void(uint32 clientID)>;

    class Server : public NonCopyable
    {
    public:
        Server()  = default;
        ~Server() { Stop(); }

        bool Start(uint16 port, int32 maxClients = 32);
        void Stop();
        void Update();

        bool SendToClient(uint32 clientID, const Packet& packet);
        void BroadcastPacket(const Packet& packet, uint32 excludeID = 0);
        void DisconnectClient(uint32 clientID);

        void SetPacketHandler(ServerPacketHandler h)     { m_PacketHandler = std::move(h); }
        void SetConnectHandler(ClientConnectHandler h)   { m_ConnectHandler = std::move(h); }
        void SetDisconnectHandler(ClientDisconnectHandler h){ m_DisconnectHandler = std::move(h); }

        bool     IsRunning()    const { return m_Running; }
        uint32   GetClientCount()const;

    private:
        void AcceptClients();
        void ReceiveFromClients();

        Socket m_ListenSocket;
        std::unordered_map<uint32, Ref<ClientConnection>> m_Clients;
        bool    m_Running{false};
        uint32  m_NextClientID{1};
        int32   m_MaxClients{32};
        std::vector<byte> m_Buffer;

        ServerPacketHandler     m_PacketHandler;
        ClientConnectHandler    m_ConnectHandler;
        ClientDisconnectHandler m_DisconnectHandler;
    };

} // namespace Good