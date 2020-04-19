#include "network-layer.hpp"

#include <iostream>

namespace platform {

void NetworkLayer::run() {
    while (!should_exit()) {
        if (m_server->wait_with_timeout(1000)) {
            std::string msg = m_server->receive();
            std::cout << "NetworkLayer: received message " << msg << std::endl;
        } else {
            std::cout << "NetworkLayer: timed out with no msg" << std::endl;
        }
    }
}

void NetworkLayer::exit() {
    m_exit_signal.set_value();
}

bool NetworkLayer::should_exit() {
    if (m_exit_check.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout) {
        return false;
    }
    return true;
}

} // namespace platform