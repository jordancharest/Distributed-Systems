#include "swim-process.hpp"

#include <chrono>
#include <iostream>
#include <random>
#include <sstream>

#include "msg/swim.pb.h"

namespace swim {

namespace {
static constexpr unsigned int kMaxRetries = 5;
static constexpr unsigned int kDefaultMsgTimeout_ms = 1000;
static constexpr unsigned int kRunPeriod_ms = 1000;
static constexpr unsigned int kPingPeriod_ms = 5000;
} // anonymous namespace


std::string SwimProcess::log_header(LogLevel lvl) {
    std::ostringstream oss;
    using namespace std::chrono;
    oss << duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();

    auto print_member_map = [&]() {
        for (auto itr = m_members.begin(); itr != m_members.end(); ++itr) {
            oss << itr->first << ":" << itr->second.port();
            if (std::next(itr) != m_members.end()) {
                oss << ", ";
            }
        }
        oss << "\n";
    };

    auto print_updates = [&]() {
        for (auto itr = m_recent_updates.begin(); itr != m_recent_updates.end(); ++itr) {
            oss << "{" << itr->member.id() << ":" << itr->member.port() << ", rem: " << itr->retransmit << "}";
            if (std::next(itr) != m_recent_updates.end()) {
                oss << ", ";
            }
        }
        oss << "\n";
    };





    switch (lvl) {
        case LOG_DEBUG:
            // print everything
            oss << " DBG :\n"
                << "                   members: "; print_member_map();
            oss << "                   updates: "; print_updates();
            oss << "                   ";
            break;
        case LOG_INFO:
            oss << " INFO: ";
            break;
        case LOG_FATAL:
            oss << " ERROR: ";
            break;

    }

    return oss.str();
}


void SwimProcess::run() {
    // add self to membership list
    add_member(m_id, m_server->port());

    bool initialized = false;
    while (!should_crash()) {
        auto start = std::chrono::steady_clock::now();
        if (!initialized) {
            initialize();
            std::cout << log_header() << "initialized" << std::endl;
            initialized = true;
        }

        execute_swim();

        // sleep for the remainder of the execution period
        std::this_thread::sleep_for(std::chrono::milliseconds(kRunPeriod_ms) - std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now()-start));
    }
    std::cout << log_header() << "crashed" << std::endl;
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
                std::cout << log_header() << "recieved PING. Sending ACK" << std::endl;
                add_member(swim_msg.sender_id(), port);
                send_message(ip, port, Message::ACK);
                break;
            case Message::ACK:
                std::cout << log_header() << "received ACK. Do nothing for now." << std::endl;
                break;
            default:
                std::cout << log_header(LOG_FATAL) << "Unknown message type" << std::endl;
                std::exit(1);
        }
}

void SwimProcess::send_ping() {
    static unsigned int ping_index = 0;

    // this process is the only member
    if (m_id_list.size() <= 1) {
        return;
    }

    // permute the list if we are past the end or we are at the last element and it's this process
    if (ping_index >= m_id_list.size()
        || (m_members.at(m_id_list[ping_index]).port() == m_server->port() && ping_index + 1 >= m_id_list.size())) {
        // randomly permute the member list for the next round of pings
        std::random_device rd;
        std::mt19937 generator(rd());
        std::shuffle(m_id_list.begin(), m_id_list.end(), generator);
        ping_index = 0;

    }

    // don't send pings to yourself
    if (m_members.at(m_id_list[ping_index]).port() == m_server->port()) {
        ++ping_index;
    }

    unsigned int recipient_port = m_members.at(m_id_list[ping_index]).port();
    ++ping_index;
    std::cout << log_header() << "send ping to 127.0.0.1:" << recipient_port << std::endl;
    send_message("127.0.0.1", recipient_port, Message::PING);
}


void SwimProcess::initialize() {
    // send a join request to the coordinator
    if (!m_is_coordinator) {
        if (!attempt_join()) {
            std::cerr << log_header(LOG_FATAL) << "Unable to join. Exiting." << std::endl;
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
                std::cout << log_header(LOG_FATAL) << "Received msg before any members have joined" << std::endl;
                std::exit(1);
        }
    }
}


void SwimProcess::send_message(const std::string& ip, unsigned int port, Message::Type msg_type) {
    // apparently performs better if you avoid allocations, I didn't measure
    static Message proto_msg;
    std::string msg;
    proto_msg.set_type(msg_type);
    proto_msg.set_sender_id(m_id);

    if (!m_recent_updates.empty()) {
        for (auto& u : m_recent_updates) {
            Member* member = proto_msg.add_member();
            member->set_status(u.member.status());
            member->set_id(u.member.id());
            member->set_port(u.member.port());
            --u.retransmit;
        }
    }

    // remove any updates that are no longer recent
    while (m_recent_updates.front().retransmit <= 0) {
        m_recent_updates.pop_front();
    }

    proto_msg.SerializeToString(&msg);
    m_server->send(ip, port, msg);
}


bool SwimProcess::attempt_join() {
    std::cout << log_header() << "attempting join" << std::endl;
    for (size_t i = 0; i < kMaxRetries; ++i, std::cout << log_header() << "re-attempting join" << std::endl) {

        send_message(m_coord_host, m_coord_port, Message::PING);
        if (m_server->message_waiting(kDefaultMsgTimeout_ms)) {
            auto [rcvd_msg, ip, port] = m_server->receive_from();

            // UDP so we need to check if this is from the coordinator
            if (ip == m_coord_host && port == m_coord_port) {
                Message swim_msg;
                swim_msg.ParseFromString(rcvd_msg);
                if (swim_msg.type() == Message::ACK) {
                    std::cout << log_header() << "successfully joined" << std::endl;
                    return true;
                }
            }
        }
    }
    return false;
}


void SwimProcess::add_member(unsigned int id, unsigned int port) {
    auto existing_member = m_members.find(id);
    if (existing_member != m_members.end()) {
        // TODO: update existing member
        std::cout << log_header(LOG_DEBUG) << "not adding " << id << ". Already a member." << std::endl;
    } else {    // new member
        std::cout << log_header() << "adding member " << id << ", port " << port << std::endl;
        Member m;
        m.set_port(port);
        m.set_status(Member::ALIVE);
        m.set_id(id);

        m_members[id] = m;
        m_id_list.push_back(id);
        m_recent_updates.push_back({m});

        std::cout << log_header(LOG_DEBUG) << "added member" << std::endl;
    }
}



} // namespace swim
