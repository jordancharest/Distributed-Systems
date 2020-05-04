#include "swim-process.hpp"

#include <iostream>

#include "msg/swim.pb.h"

namespace swim {

static constexpr int kMaxRetries = 5;
static constexpr unsigned int kDefaultTimeoutMS = 500;

void SwimProcess::run() {
    // add self to membership list
    add_member(pthread_self(), m_server->port());

    bool initialized = false;
    while (!should_crash()) {
        if (!initialized) {
            initialize();
            initialized = true;
        }




        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "SWIM: crashed" << std::endl;
}


void SwimProcess::initialize() {
    // send a join request to the coordinator
    if (!m_is_coordinator && !attempt_join()) {
        std::cerr << "SWIM: unable to join. Exiting." << std::endl;
        std::exit(1);
    } else {    // this is the coordinator, wait for at least one process to join
        auto [msg, ip, port] = m_server->receive_from();
        swim::Join join_msg;
        join_msg.ParseFromString(msg);

        switch (join_msg.type()) {
            case Join::REQUEST:
                add_member(0, port);
                send_join_message(ip, port, Join::ACK);
                break;
            default:
                std::cout << "Received message before join request. Ignoring." << std::endl; 
        }
    }
}


void SwimProcess::send_join_message(const std::string& ip, unsigned int port, Join::Type msg_type) {
    // apparently performs better if you avoid allocations, I didn't measure
    static Join join_msg;
    std::string msg;
    join_msg.set_type(msg_type);
    join_msg.SerializeToString(&msg);
    m_server->send(ip, port, msg);
}


bool SwimProcess::attempt_join() {
    std::cout << "SWIM: attempting join" << std::endl;
    for (size_t i = 0; i < kMaxRetries; ++i, std::cout << "SWIM: re-attempting join" << std::endl) {
        send_join_message(m_coord_host, m_coord_port, Join::REQUEST);
        if (m_server->message_waiting(kDefaultTimeoutMS)) {
            auto [rcvd_msg, ip, port] = m_server->receive_from();

            // UDP so we need to check if this is from the coordinator
            if (ip == m_coord_host && port == m_coord_port) {
                Join join_msg;
                join_msg.ParseFromString(rcvd_msg);
                if (join_msg.type() == Join::ACK) {
                    std::cout << "SWIM: successfully joined" << std::endl;
                    return true;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return false;
}


void SwimProcess::add_member(unsigned int id, unsigned int port) {
    // need to do other stuff, but for now, just add it
    std::cout << "Adding member " << id << " at port " << port << std::endl;
    m_members[id] = port;
}



} // namespace swim
