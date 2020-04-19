#include "faulty-process.hpp"

#include <iostream>


namespace platform {

FaultyProcess::FaultyProcess(std::shared_ptr<UdpServer> server) :
    m_network_layer(server),
    m_crash_check(m_crash_signal.get_future())
{
    std::cout << "Initializing network layer" << std::endl;
    m_network_thread = std::thread([&](){ m_network_layer.run(); });
}

FaultyProcess::~FaultyProcess() {
    std::cout << "Tearing down network layer" << std::endl;
    m_network_layer.exit();
    m_network_thread.join();
}

void FaultyProcess::run() {
    std::cout << "FP: starting new instance" << std::endl;

    while (!should_crash()) {
        // run algorithm
        std::cout << "FP: running" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "FP: crashed" << std::endl;
}

void FaultyProcess::crash() {
    m_crash_signal.set_value();
}

bool FaultyProcess::should_crash() {
    // checks if value in future object is available
    if (m_crash_check.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout) {
        return false;
    }
    return true;
}

} // namespace platform