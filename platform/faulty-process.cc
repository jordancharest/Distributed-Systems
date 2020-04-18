#include "faulty-process.hpp"

#include <iostream>

namespace platform {

void FaultyProcess::run() {
    std::cout << "FP: starting new instance" << std::endl;

    while (!should_crash()) {
        // receive a message (blocking, so don't actually call it here)
        // m_server.receive();

        // run algorithm
        std::cout << "FP: running" << std::endl;

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    std::cout << "FP: crashed" << std::endl;
}


bool FaultyProcess::should_crash() {
    // checks if value in future object is available
    if (m_crash_check.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
        return false;
    return true;
}

void FaultyProcess::crash() {
    m_crash_signal.set_value();
}

} // namespace platform