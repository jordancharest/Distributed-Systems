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
    FaultyProcess(std::shared_ptr<UdpServer> server);

    /** Copying is forbidden. std::threads can't be copied */
    FaultyProcess(const FaultyProcess&) = delete;
    FaultyProcess& operator=(const FaultyProcess&) = delete;

    /** 
     * Run the distributed algorithm. Be sure to periodically check
     * @ref should_crash() to know when to exit.
     */
    virtual void run();

    /** Signal to this thread that it should "crash" (exit) */
    void crash();
protected:
    // Check if this thread should exit
    bool should_crash();

    std::shared_ptr<UdpServer> m_server;

private:
    // manage the exit crash signal
    std::promise<void> m_crash_signal;
    std::future<void> m_crash_check;
};
} // namespace platform