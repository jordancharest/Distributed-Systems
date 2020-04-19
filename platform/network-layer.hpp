#include <future>
#include <memory>

#include "udp-server.hpp"


namespace platform {

class NetworkLayer {
public:
    NetworkLayer(std::shared_ptr<UdpServer> server) :
        m_server(server),
        m_exit_check(m_exit_signal.get_future())
        {}

    /** Run a loop that accepts network messages and places them on the common buffer */
    void run();

    /** Send a signal to the network layer to exit */
    void exit();

private:
    // Check if this thread should exit
    bool should_exit();
   
    std::promise<void> m_exit_signal;
    std::future<void> m_exit_check;
    std::shared_ptr<UdpServer> m_server;
};
} // namespace platform