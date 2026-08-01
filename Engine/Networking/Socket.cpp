#include "Socket.h"
#include "../Core/Log.h"

namespace Good
{

bool Socket::InitNetwork()
{
#if defined(GOOD_PLATFORM_WINDOWS)
    WSADATA wsa;
    int result = WSAStartup(MAKEWORD(2,2), &wsa);
    if (result != 0)
    {
        GOOD_LOG_ERROR("Network", "WSAStartup failed: {}", result);
        return false;
    }
    GOOD_LOG_INFO("Network", "Winsock initialized");
    return true;
#else
    GOOD_LOG_INFO("Network", "Network initialized");
    return true;
#endif
}

void Socket::ShutdownNetwork()
{
#if defined(GOOD_PLATFORM_WINDOWS)
    WSACleanup();
    GOOD_LOG_INFO("Network", "Winsock shutdown");
#else
    GOOD_LOG_INFO("Network", "Network shutdown");
#endif
}

bool Socket::Create(SocketType type)
{
    m_Type = type;
    int sockType = (type == SocketType::TCP) ? SOCK_STREAM : SOCK_DGRAM;
    m_Handle = socket(AF_INET, sockType, 0);
    if (!IsValid())
    {
        GOOD_LOG_ERROR("Network", "Socket creation failed");
        return false;
    }
    return true;
}

bool Socket::Bind(const std::string& ip, uint16 port)
{
    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = ip.empty() ? INADDR_ANY : inet_addr(ip.c_str());

    if (bind(m_Handle, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0)
    {
        GOOD_LOG_ERROR("Network", "Bind failed on {}:{}", ip, port);
        return false;
    }

    GOOD_LOG_INFO("Network", "Bound to {}:{}", ip.empty() ? "0.0.0.0" : ip, port);
    return true;
}

bool Socket::Connect(const std::string& ip, uint16 port)
{
    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (connect(m_Handle, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0)
    {
        GOOD_LOG_ERROR("Network", "Connect failed to {}:{}", ip, port);
        return false;
    }

    GOOD_LOG_INFO("Network", "Connected to {}:{}", ip, port);
    return true;
}

bool Socket::Listen(int32 backlog)
{
    if (listen(m_Handle, backlog) != 0)
    {
        GOOD_LOG_ERROR("Network", "Listen failed");
        return false;
    }
    return true;
}

bool Socket::Accept(Socket& outClient)
{
    sockaddr_in clientAddr{};
    socklen_t len = sizeof(clientAddr);
    SocketHandle clientHandle = accept(m_Handle,
        reinterpret_cast<sockaddr*>(&clientAddr), &len);

    if (clientHandle == INVALID_SOCKET_HANDLE) return false;
    outClient.m_Handle = clientHandle;
    return true;
}

void Socket::Close()
{
    if (!IsValid()) return;
#if defined(GOOD_PLATFORM_WINDOWS)
    closesocket(m_Handle);
#else
    close(m_Handle);
#endif
    m_Handle = INVALID_SOCKET_HANDLE;
}

bool Socket::SetNonBlocking(bool nonBlocking)
{
#if defined(GOOD_PLATFORM_WINDOWS)
    u_long mode = nonBlocking ? 1 : 0;
    return ioctlsocket(m_Handle, FIONBIO, &mode) == 0;
#else
    int flags = fcntl(m_Handle, F_GETFL, 0);
    if (nonBlocking) flags |= O_NONBLOCK;
    else             flags &= ~O_NONBLOCK;
    return fcntl(m_Handle, F_SETFL, flags) == 0;
#endif
}

bool Socket::SetReuseAddress(bool reuse)
{
    int val = reuse ? 1 : 0;
    return setsockopt(m_Handle, SOL_SOCKET, SO_REUSEADDR,
                      reinterpret_cast<const char*>(&val), sizeof(val)) == 0;
}

int32 Socket::Send(const void* data, usize size)
{
    return static_cast<int32>(send(m_Handle,
        static_cast<const char*>(data), static_cast<int>(size), 0));
}

int32 Socket::Receive(void* buffer, usize bufferSize)
{
    return static_cast<int32>(recv(m_Handle,
        static_cast<char*>(buffer), static_cast<int>(bufferSize), 0));
}

int32 Socket::SendTo(const void* data, usize size, const std::string& ip, uint16 port)
{
    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    return static_cast<int32>(sendto(m_Handle,
        static_cast<const char*>(data), static_cast<int>(size), 0,
        reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)));
}

int32 Socket::ReceiveFrom(void* buffer, usize bufferSize, std::string& outIP, uint16& outPort)
{
    sockaddr_in addr{};
    socklen_t len = sizeof(addr);

    int32 received = static_cast<int32>(recvfrom(m_Handle,
        static_cast<char*>(buffer), static_cast<int>(bufferSize), 0,
        reinterpret_cast<sockaddr*>(&addr), &len));

    if (received > 0)
    {
        outIP   = inet_ntoa(addr.sin_addr);
        outPort = ntohs(addr.sin_port);
    }

    return received;
}

} // namespace Good