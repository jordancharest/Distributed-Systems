#pragma once

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <tuple>

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
     * Checks to see if there is a message in the network buffer waiting to be received.
     * Returns when a message is received or the timeout is reached. The message remains
     * in the network buffer and can then be received by calling @ref receive.
     * @param [in] timeout_ms Max time to wait without receiving a message in milliseconds. Defaults to 0.
     * Returns true if there is a message waiting.
     */
    bool message_waiting(unsigned int timeout_ms = 0);

    /**
    * Send a message to designated recipient
    * @param [in] server_ip ip address of the recipient
    * @param [in] server_port port of the recipient
    * @param [in] buffer the message to send
    */
    void send(const std::string& ip, int port, const std::string& buffer);
    
    /**
    * Blocking wait to receive message. Can be called when @ref message_waiting() returns true
    * to immediately receive a message without blocking.
    * @return the received message
    */
    std::string receive();

    /** Same as @ref receive, but also return the sender's ip address and port */
    std::tuple<std::string, std::string, unsigned int> receive_from();

    /** Get the port this server is bound to */
    unsigned int port() const { return m_port; }

private:
    int m_sd;
    struct sockaddr_in m_sockaddr;
    unsigned int m_port;
    std::string m_host;
};
} // namespace platform