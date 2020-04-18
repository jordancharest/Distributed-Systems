#pragma once

#include <thread>
#include <future>
#include "udp-server.hpp"

namespace platform {

class FaultyProcess {
public:
    /** Constructor.
     * @param [in] server ptr to a server object that sends and receives messages
     */
    FaultyProcess(std::shared_ptr<UdpServer> server) :
        m_server(server),
        m_crash_check(m_crash_signal.get_future())
        {}

    /** Copying is forbidden. std::thread's can't be copied */
    FaultyProcess(const FaultyProcess&) = delete;
    FaultyProcess& operator=(const FaultyProcess&) = delete;

    /** 
     * Run the distributed algorithm. Be sure to periodically check
     * @ref should_crash() to know when to exit.
     */
    virtual void run();

    /** Check if this thread should exit */
    bool should_crash();

    /** Signal to this thread that it should "crash" (exit) */
    void crash();

private:
    std::shared_ptr<UdpServer> m_server;
    std::promise<void> m_crash_signal;
    std::future<void> m_crash_check;
};
} // namespace platform