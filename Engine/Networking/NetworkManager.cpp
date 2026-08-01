#include "NetworkManager.h"
#include "../Core/Log.h"

namespace Good
{

    NetworkManager& NetworkManager::Get()
    {
        static NetworkManager s_Instance;
        return s_Instance;
    }

    bool NetworkManager::Initialize()
    {
        if (m_Initialized) return true;

        if (!Socket::InitNetwork())
        {
            GOOD_LOG_ERROR("Network", "Failed to initialize network");
            return false;
        }

        m_Initialized = true;
        GOOD_LOG_INFO("Network", "NetworkManager initialized");
        return true;
    }

    void NetworkManager::Shutdown()
    {
        if (!m_Initialized) return;
        Disconnect();
        Socket::ShutdownNetwork();
        m_Initialized = false;
        GOOD_LOG_INFO("Network", "NetworkManager shutdown");
    }

    void NetworkManager::Update()
    {
        if (IsServer()) m_Server.Update();
        if (IsClient()) m_Client.Update();
    }

    bool NetworkManager::StartServer(uint16 port, int32 maxClients)
    {
        if (!m_Server.Start(port, maxClients)) return false;
        m_Mode = NetworkMode::Server;
        return true;
    }

    bool NetworkManager::ConnectToServer(const std::string& ip, uint16 port)
    {
        if (!m_Client.Connect(ip, port)) return false;
        m_Mode = NetworkMode::Client;
        return true;
    }

    void NetworkManager::Disconnect()
    {
        m_Server.Stop();
        m_Client.Disconnect();
        m_Mode = NetworkMode::None;
    }

    bool NetworkManager::IsConnected() const
    {
        if (m_Mode == NetworkMode::Client) return m_Client.IsConnected();
        if (m_Mode == NetworkMode::Server) return m_Server.IsRunning();
        return false;
    }

} // namespace Good