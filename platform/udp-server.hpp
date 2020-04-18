#pragma once

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

namespace platform {

class UdpServer {
public:
    /**
    * Constructor.
    * @param [in] host hostname of this server
    * @param [in] port port this server is listening on
    */
    UdpServer(std::string host, int port);

    /** Default detructor. */
    ~UdpServer() = default;

    /** Copy and move are forbidden */
    UdpServer(const UdpServer& rhs) = delete;
    UdpServer& operator=(const UdpServer& rhs) = delete;

    /**
    * Send a message to designated recipient
    * @param [in] server_ip ip address of the recipient
    * @param [in] server_port port of the recipient
    * @param [in] buffer the message to send
    */
    void send(const std::string& ip, int port, const std::string& buffer);
    
    /**
    * Blocking wait to receive message.
    * @return the received message
    */
    std::string receive();

private:
    int m_sd;
    struct sockaddr_in m_sockaddr;
    int m_port;
    std::string m_host;
};
} // namespace platform