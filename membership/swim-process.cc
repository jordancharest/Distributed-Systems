#include "swim-process.hpp"

#include <chrono>
#include <iostream>

#include "msg/swim.pb.h"

namespace swim {

static constexpr int kMaxRetries = 5;
static constexpr unsigned int kDefaultTimeout_ms = 1000;
static constexpr unsigned int kRunPeriod_ms = 1000;

void SwimProcess::run() {
    // add self to membership list
    add_member(pthread_self(), m_server->port());

    bool initialized = false;
    while (!should_crash()) {
        auto start = std::chrono::steady_clock::now();
        if (!initialized) {
            initialize();
            initialized = true;
        }

        execute_swim();

        // sleep for the remainder of the execution period
        std::this_thread::sleep_for(std::chrono::milliseconds(kRunPeriod_ms) - std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now()-start));
    }
    std::cout << "SWIM: crashed" << std::endl;
}

void SwimProcess::execute_swim() {
    if (m_server->message_waiting(0)) {
        auto [msg, ip, port] = m_server->receive_from();
        swim::Message swim_msg;
        swim_msg.ParseFromString(msg);
        switch (swim_msg.type()) {
            case Message::PING:
                add_member(swim_msg.sender_id(), port);
                send_join_message(ip, port, Message::ACK);
                break;
            default:
                std::cout << "Received message before join request. Ignoring." << std::endl; 
        }



    }






}


void SwimProcess::initialize() {
    // send a join request to the coordinator
    if (!m_is_coordinator && !attempt_join()) {
        std::cerr << "SWIM: unable to join. Exiting." << std::endl;
        std::exit(1);
    } else {    // this is the coordinator, wait for at least one process to join
        auto [msg, ip, port] = m_server->receive_from();
        Message swim_msg;
        swim_msg.ParseFromString(msg);

        switch (swim_msg.type()) {
            case Message::PING:
                add_member(swim_msg.sender_id(), port);
                send_join_message(ip, port, Message::ACK);
                break;
            default:
                std::cout << "Received message before join request. Ignoring." << std::endl; 
        }
    }
}


void SwimProcess::send_join_message(const std::string& ip, unsigned int port, Message::Type msg_type) {
    // apparently performs better if you avoid allocations, I didn't measure
    static Message swim_msg;
    std::string msg;
    swim_msg.set_type(msg_type);
    swim_msg.set_sender_id(pthread_self());
    swim_msg.SerializeToString(&msg);
    m_server->send(ip, port, msg);
}


bool SwimProcess::attempt_join() {
    std::cout << "SWIM: attempting join" << std::endl;
    for (size_t i = 0; i < kMaxRetries; ++i, std::cout << "SWIM: re-attempting join" << std::endl) {

        send_join_message(m_coord_host, m_coord_port, Message::PING);
        if (m_server->message_waiting(kDefaultTimeout_ms)) {
            auto [rcvd_msg, ip, port] = m_server->receive_from();

            // UDP so we need to check if this is from the coordinator
            if (ip == m_coord_host && port == m_coord_port) {
                Message swim_msg;
                swim_msg.ParseFromString(rcvd_msg);
                if (swim_msg.type() == Message::ACK) {
                    std::cout << "SWIM: successfully joined" << std::endl;
                    return true;
                }
            }
        }
        std::cout << "SWIM: no response to join request" << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return false;
}


void SwimProcess::add_member(unsigned int id, unsigned int port) {
    // need to do other stuff, but for now, just add it
    std::cout << "SWIM: adding member " << id << " at port " << port << std::endl;
    m_members[id] = port;
}



} // namespace swim
