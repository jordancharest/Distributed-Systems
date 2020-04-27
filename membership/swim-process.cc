#include "swim-process.hpp"

#include <iostream>

#include "msg/swim.pb.h"

namespace swim {

void SwimProcess::run () {
    std::cout << "SWIM: starting new instance" << std::endl;

    // send a join request to the coordinator
    if (!m_is_coordinator) {
        std::cout << "SWIM: sending join request\n";

        swim::Join join_msg;
        join_msg.set_type(Join_Type_REQUEST);
        std::cout << " SWIM: Join msg:\n" << join_msg.DebugString() << std::endl;


        std::string msg;
        join_msg.SerializeToString(&msg);
        m_server->send(m_coord_host, m_coord_port, msg);
    }

    while (!should_crash()) {
        // run algorithm
        // if (m_server->message_waiting()) {
        if (m_is_coordinator) {
            std::string msg = m_server->receive();
            std::cout << "SWIM: received a message: " << msg << std::endl;
        }
        // } else {
            // std::cout << "SWIM: no message" << std::endl;
        // }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "SWIM: crashed" << std::endl;
}

} // namespace swim
