#include "udp-server.hpp"

#include <errno.h>
#include <string.h>

#include <iostream>

namespace platform {

static constexpr int kMaxBuffer = 1024;

// ----------------------------------------------------------------------------
UdpServer::UdpServer(std::string host, int port) : m_host(std::move(host)), m_port(port) {
    // create the socket: IPv4, UDP
    if ((m_sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "ERROR: " << strerror(errno) << "\nsocket() failed" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    m_sockaddr.sin_family = AF_INET; // IPv4
    m_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);  // accept any address

    // specify port of this server
    m_sockaddr.sin_port = htons(m_port);

    // bind to port
    if (bind(m_sd, (struct sockaddr* )&m_sockaddr, sizeof(m_sockaddr)) < 0) {
        std::cerr << "ERROR: " << strerror(errno) << "\nbind() failed" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

// ----------------------------------------------------------------------------
bool UdpServer::message_waiting(unsigned int timeout_ms) {
    struct timeval timeout = {timeout_ms / 1000, (timeout_ms % 1000) * 1000}; // seconds, microseconds

    // tell which descriptors to wait for (only want the socket descriptor for this server)
    fd_set read_fd_set;
    FD_ZERO(&read_fd_set);
    FD_SET(m_sd, &read_fd_set);

    int ready = select(FD_SETSIZE, &read_fd_set, NULL, NULL, &timeout);
    return FD_ISSET(m_sd, &read_fd_set);
}

// ----------------------------------------------------------------------------
void UdpServer::send(const std::string& ip, int port, const std::string& buffer) {
    struct sockaddr_in server;
    struct hostent* hp;
    server.sin_family = AF_INET;

    if ((hp = gethostbyname(ip.c_str())) == NULL) {
        std::cerr << "ERROR: " << strerror(errno) << "\ngethostbyname() failed" << std::endl;
    }
    memcpy(&server.sin_addr.s_addr, hp->h_addr, hp->h_length);

    // establish the server port number - we must use network byte order!
    server.sin_port = htons(port);

    if (sendto(m_sd, buffer.c_str(), buffer.size(), 0, (struct sockaddr*)&server, sizeof server) < 0) {
        std::cerr << "ERROR: " << strerror(errno) << "\nsendto() failed" << std::endl;
    }
}

// ----------------------------------------------------------------------------
std::string UdpServer::receive() {
    char buffer[kMaxBuffer];
    struct sockaddr_in client;
    int len = sizeof client;

    // recvfrom is a blocking call
    int n_bytes = recvfrom(m_sd, buffer, kMaxBuffer, 0, (struct sockaddr*)&client, (socklen_t*)&len);
    if (n_bytes < 0) {
        std::cerr << "ERROR: " << strerror(errno) << "\nrecvfrom() failed" << std::endl;
    } else {
        std::cout << "Received datagram from: " << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << "\n";

        // null terminate and copy it to the return string
        buffer[n_bytes] = '\0';
    }
    return buffer;
}

// ----------------------------------------------------------------------------
std::tuple<std::string, std::string, unsigned int>
UdpServer::receive_from() {
    char buffer[kMaxBuffer];
    struct sockaddr_in client;
    int len = sizeof client;

    // recvfrom is a blocking call
    int n_bytes = recvfrom(m_sd, buffer, kMaxBuffer, 0, (struct sockaddr*)&client, (socklen_t*)&len);
    if (n_bytes < 0) {
        std::cerr << "ERROR: " << strerror(errno) << "\nrecvfrom() failed" << std::endl;
    } else {
        std::cout << "Received datagram from: " << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << "\n";

        // null terminate and copy it to the return string
        buffer[n_bytes] = '\0';
    }
    return {buffer, inet_ntoa(client.sin_addr), ntohs(client.sin_port)};   
}

} // namespace platform