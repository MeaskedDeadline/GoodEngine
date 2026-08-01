#pragma once
#include "../Core/Core.h"

#if defined(GOOD_PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

namespace Good
{

#if defined(GOOD_PLATFORM_WINDOWS)
    using SocketHandle = SOCKET;
    constexpr SocketHandle INVALID_SOCKET_HANDLE = INVALID_SOCKET;
#else
    using SocketHandle = int;
    constexpr SocketHandle INVALID_SOCKET_HANDLE = -1;
#endif

    enum class SocketType : uint8 { TCP, UDP };

    class Socket
    {
    public:
        Socket()  = default;
        ~Socket() { Close(); }

        static bool InitNetwork();
        static void ShutdownNetwork();

        bool Create(SocketType type = SocketType::TCP);
        bool Bind(const std::string& ip, uint16 port);
        bool Connect(const std::string& ip, uint16 port);
        bool Listen(int32 backlog = 5);
        bool Accept(Socket& outClient);
        void Close();

        bool SetNonBlocking(bool nonBlocking);
        bool SetReuseAddress(bool reuse);

        int32 Send(const void* data, usize size);
        int32 Receive(void* buffer, usize bufferSize);

        int32 SendTo(const void* data, usize size, const std::string& ip, uint16 port);
        int32 ReceiveFrom(void* buffer, usize bufferSize, std::string& outIP, uint16& outPort);

        bool IsValid()    const { return m_Handle != INVALID_SOCKET_HANDLE; }
        SocketHandle GetHandle() const { return m_Handle; }

    private:
        SocketHandle m_Handle{INVALID_SOCKET_HANDLE};
        SocketType   m_Type{SocketType::TCP};
    };

} // namespace Good