#include "Server.h"
#include "../Core/Log.h"

namespace Good
{

bool Server::Start(uint16 port, int32 maxClients)
{
    m_MaxClients = maxClients;
    m_Buffer.resize(65536);

    if (!m_ListenSocket.Create(SocketType::TCP))    return false;
    m_ListenSocket.SetReuseAddress(true);
    if (!m_ListenSocket.Bind("", port))             return false;
    if (!m_ListenSocket.Listen(maxClients))         return false;
    m_ListenSocket.SetNonBlocking(true);

    m_Running = true;
    GOOD_LOG_INFO("Network", "Server started on port {}", port);
    return true;
}

void Server::Stop()
{
    if (!m_Running) return;
    m_Clients.clear();
    m_ListenSocket.Close();
    m_Running = false;
    GOOD_LOG_INFO("Network", "Server stopped");
}

void Server::Update()
{
    if (!m_Running) return;
    AcceptClients();
    ReceiveFromClients();
}

void Server::AcceptClients()
{
    if (static_cast<int32>(m_Clients.size()) >= m_MaxClients) return;

    auto conn = MakeRef<ClientConnection>();
    if (!m_ListenSocket.Accept(conn->socket)) return;

    conn->id = m_NextClientID++;
    conn->socket.SetNonBlocking(true);

    m_Clients[conn->id] = conn;
    GOOD_LOG_INFO("Network", "Client #{} connected", conn->id);

    if (m_ConnectHandler) m_ConnectHandler(conn->id);
}

void Server::ReceiveFromClients()
{
    std::vector<uint32> toRemove;

    for (auto& [id, conn] : m_Clients)
    {
        if (!conn->connected) { toRemove.push_back(id); continue; }

        uint32 size = 0;
        int32 received = conn->socket.Receive(&size, sizeof(size));

        if (received == 0) { toRemove.push_back(id); continue; }
        if (received < 0)  continue;

        if (size == 0 || size > 65536) continue;

        m_Buffer.resize(size);
        received = conn->socket.Receive(m_Buffer.data(), size);
        if (received <= 0) { toRemove.push_back(id); continue; }

        if (m_PacketHandler)
        {
            Packet packet;
            packet.Write(m_Buffer.data(), static_cast<usize>(received));
            packet.Reset();
            m_PacketHandler(id, packet);
        }
    }

    for (uint32 clientID : toRemove)
    {
        GOOD_LOG_INFO("Network", "Client #{} disconnected", clientID);
        if (m_DisconnectHandler) m_DisconnectHandler(clientID);
        m_Clients.erase(clientID);
    }
}

bool Server::SendToClient(uint32 clientID, const Packet& packet)
{
    auto it = m_Clients.find(clientID);
    if (it == m_Clients.end() || !it->second->connected) return false;

    uint32 size = static_cast<uint32>(packet.GetSize());
    it->second->socket.Send(&size, sizeof(size));
    return it->second->socket.Send(packet.GetData(), packet.GetSize()) > 0;
}

void Server::BroadcastPacket(const Packet& packet, uint32 excludeID)
{
    for (auto& [id, conn] : m_Clients)
    {
        if (id == excludeID || !conn->connected) continue;
        SendToClient(id, packet);
    }
}

void Server::DisconnectClient(uint32 clientID)
{
    auto it = m_Clients.find(clientID);
    if (it != m_Clients.end())
    {
        it->second->socket.Close();
        it->second->connected = false;
    }
}

uint32 Server::GetClientCount() const
{
    return static_cast<uint32>(m_Clients.size());
}

} // namespace Good