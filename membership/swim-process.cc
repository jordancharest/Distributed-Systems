#include "swim-process.hpp"

#include <chrono>
#include <iostream>
#include <random>

#include "msg/swim.pb.h"

namespace swim {

static constexpr unsigned int kMaxRetries = 5;
static constexpr unsigned int kDefaultMsgTimeout_ms = 1000;
static constexpr unsigned int kRunPeriod_ms = 1000;
static constexpr unsigned int kPingPeriod_ms = 5000;


void log(std::string) {
    // auto t = std::chrono::steady_clock::now();

    // use std::put_time()

}



void SwimProcess::run() {
    // add self to membership list
    add_member(m_id, m_server->port());

    bool initialized = false;
    while (!should_crash()) {
        auto start = std::chrono::steady_clock::now();
        if (!initialized) {
            initialize();
            std::cout << "SWIM: finished initialization" << std::endl;
            initialized = true;
        }

        execute_swim();

        // sleep for the remainder of the execution period
        std::this_thread::sleep_for(std::chrono::milliseconds(kRunPeriod_ms) - std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now()-start));
    }
    std::cout << "SWIM: crashed" << std::endl;
}

void SwimProcess::execute_swim() {
    if (m_server->message_waiting()) {
        handle_message();
    }

    using namespace std::chrono;
    static auto last_ping = steady_clock::now();
    if (duration_cast<milliseconds>(steady_clock::now() - last_ping).count() > kPingPeriod_ms) {
        send_ping();
        last_ping = steady_clock::now();
    }
}

void SwimProcess::handle_message() {
        auto [msg, ip, port] = m_server->receive_from();
        Message swim_msg;
        swim_msg.ParseFromString(msg);
        switch (swim_msg.type()) {
            case Message::PING:
                std::cout << "SWIM: recieved PING. Responding ACK." << std::endl;
                add_member(swim_msg.sender_id(), port);
                send_message(ip, port, Message::ACK);
                break;
            case Message::ACK:
                std::cout << "SWIM: received ACK. Nothing to do for now.\n" << std::endl;
                break;
            default:
                std::cout << "Received message before join request. Ignoring." << std::endl; 
        }
}

void SwimProcess::send_ping() {
    static unsigned int ping_index = 0;

    // this process is the only member
    if (m_id_list.size() <= 1) {
        return;
    }

    // don't send pings to yourself
    if (m_members[m_id_list[ping_index]].port() == m_server->port()) {
        ++ping_index;
    }

    if (ping_index >= m_id_list.size()) {
        // randomly permute the member list for the next round of pings
        std::random_device rd;
        std::mt19937 generator(rd());
        std::shuffle(m_id_list.begin(), m_id_list.end(), generator);
        ping_index = 0;
    }

    Member m = m_members[m_id_list[ping_index]];
    ++ping_index;
    std::cout << "SWIM: send ping to 127.0.0.1:" << m.port() << std::endl;
    send_message("127.0.0.1", m.port(), Message::PING);
}


void SwimProcess::initialize() {
    // send a join request to the coordinator
    if (!m_is_coordinator) {
        if (!attempt_join()) {
            std::cerr << "SWIM: unable to join. Exiting." << std::endl;
            std::exit(1);
        }
    } else {    // this is the coordinator, wait for at least one process to join
        auto [msg, ip, port] = m_server->receive_from();
        Message swim_msg;
        swim_msg.ParseFromString(msg);

        switch (swim_msg.type()) {
            case Message::PING:
                add_member(swim_msg.sender_id(), port);
                send_message(ip, port, Message::ACK);
                break;
            default:
                std::cout << "SWIM: received message before join request. Ignoring." << std::endl; 
        }
    }
}


void SwimProcess::send_message(const std::string& ip, unsigned int port, Message::Type msg_type) {
    // apparently performs better if you avoid allocations, I didn't measure
    static Message proto_msg;
    std::string msg;
    proto_msg.set_type(msg_type);
    proto_msg.set_sender_id(m_id);
    proto_msg.SerializeToString(&msg);
    m_server->send(ip, port, msg);
}


bool SwimProcess::attempt_join() {
    std::cout << "SWIM: attempting join" << std::endl;
    for (size_t i = 0; i < kMaxRetries; ++i, std::cout << "SWIM: re-attempting join" << std::endl) {

        send_message(m_coord_host, m_coord_port, Message::PING);
        if (m_server->message_waiting(kDefaultMsgTimeout_ms)) {
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
    }
    return false;
}


void SwimProcess::add_member(unsigned int id, unsigned int port) {
    auto existing_member = m_members.find(id);
    if (existing_member != m_members.end()) {
        // TODO: update existing member
        std::cout << "SWIM: Not adding " << id << ". Already a member." << std::endl;
    } else {    // new member
        std::cout << "SWIM: adding member " << id << " at port " << port << std::endl;
        Member m;
        m.set_port(port);
        m.set_status(Member::ALIVE);
        m.set_id(id);

        m_members[id] = m;
        m_id_list.push_back(id);
        m_recent_updates.push({m});
    }
}



} // namespace swim
