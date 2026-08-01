#pragma once
#include "../Core/Core.h"
#include "Client.h"
#include "Server.h"

namespace Good
{

    enum class NetworkMode : uint8 { None, Client, Server, Host };

    class NetworkManager : public NonCopyable
    {
    public:
        static NetworkManager& Get();

        bool Initialize();
        void Shutdown();
        void Update();

        bool StartServer(uint16 port, int32 maxClients = 32);
        bool ConnectToServer(const std::string& ip, uint16 port);
        void Disconnect();

        NetworkMode GetMode()      const { return m_Mode;   }
        bool        IsConnected()  const;
        bool        IsServer()     const { return m_Mode == NetworkMode::Server || m_Mode == NetworkMode::Host; }
        bool        IsClient()     const { return m_Mode == NetworkMode::Client || m_Mode == NetworkMode::Host; }

        Server& GetServer() { return m_Server; }
        Client& GetClient() { return m_Client; }

    private:
        NetworkManager() = default;

        Server      m_Server;
        Client      m_Client;
        NetworkMode m_Mode{NetworkMode::None};
        bool        m_Initialized{false};
    };

} // namespace Good