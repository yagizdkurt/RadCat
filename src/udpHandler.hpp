#pragma once
#include <thread>
#include <string>
#include <queue>
#include <mutex>
#include <atomic>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef SOCKET socket_t;
#define SOCKET_INVALID INVALID_SOCKET
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
typedef int socket_t;
#define SOCKET_INVALID -1
#endif

class UDPHandler {
#ifdef _WIN32
    bool wsa_initialized = false;
#endif

public:
    UDPHandler();
    ~UDPHandler();

    bool start();
    void stop();
    void sendData(const std::string& data);
    
    // Message queue methods
    std::string receiveData();
    bool hasData();
    bool hasQueuedMessages();
    void processIncomingData();
    size_t getQueueSize();
    void clearQueue();

private:
    socket_t socket_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t addr_len;
    int serverPort = 8080;
    
    // Message queue for incoming UDP packets
    std::queue<std::string> m_messageQueue;
    std::mutex m_queueMutex;
    std::atomic<size_t> m_queueSize{0};
    
    // Statistics
    std::atomic<uint64_t> m_packetsReceived{0};
    std::atomic<uint64_t> m_bytesReceived{0};
    
    // Internal helper methods
    std::string receiveDataDirect();    // Direct socket receive (internal use)
    static constexpr size_t MAX_QUEUE_SIZE = 100;  // Prevent memory overflow
};
