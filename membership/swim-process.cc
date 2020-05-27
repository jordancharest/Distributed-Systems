#include "swim-process.hpp"

#include <chrono>
#include <iostream>
#include <random>
#include <sstream>
#include <unordered_map>

#include "msg/swim.pb.h"

namespace swim {

namespace {
static constexpr unsigned int kMaxJoinRetries = 5;
static constexpr unsigned int kDefaultMsgTimeout_ms = 1000;
static constexpr unsigned int kRunPeriod_ms = 1000;
static constexpr unsigned int kPingPeriod_ms = 5000;
static constexpr unsigned int kPingTimeout_ms = 2000;
static constexpr unsigned int kNumIndirectPings = 3;
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

std::string status_to_str(Member::Status status) {
    static const std::unordered_map<Member::Status, std::string> status_map = {
        {Member::ALIVE, "ALIVE"},
        {Member::SUSPECTED, "SUSPECTED"},
        {Member::FAILED, "FAILED"}
    };

    return status_map.at(status);
}

Member make_member(unsigned int id, unsigned int port, Member::Status status) {
    Member m;
    m.set_id(id);
    m.set_port(port);
    m.set_status(status);
    return m;
}

void clear_member(Member& m) {
    m.clear_port();
    m.clear_status();
    m.clear_id();
}


void SwimProcess::run() {
    // add self to membership list
    update_member_list(make_member(m_id, m_server->port(), Member::ALIVE));

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
    using namespace std::chrono;
    if (m_server->message_waiting()) {
        handle_message();
    }

    static auto last_ping = steady_clock::now();
    static bool sent_requests_this_period = false;
    if (duration_cast<milliseconds>(steady_clock::now() - last_ping).count() > kPingPeriod_ms) {
        if (m_unacknowledged_ping) {
            std::cout << log_header()
                      << "ping period finished with unacknowledged ping, suspect "
                      << m_unacknowledged_member.id() << std::endl;

            m_unacknowledged_member.set_status(Member::SUSPECTED);
            m_members[m_unacknowledged_member.id()].set_status(Member::SUSPECTED);
            m_recent_updates.push_back({m_unacknowledged_member});
            // TODO:  only add suspected process to m_recent_updates if it's not already there?
        }

        if (send_ping())
        {
            last_ping = steady_clock::now();
            m_unacknowledged_ping = true;
            sent_requests_this_period = false;
        }
    }

    // send indirect pings if the last ping timed out
    if (m_unacknowledged_ping
        && duration_cast<milliseconds>(steady_clock::now() - last_ping).count() > kPingTimeout_ms
        && !sent_requests_this_period) {

        std::cout << log_header() << "unacknowledged ping. Sending requests" << std::endl;
        send_ping_requests();
        sent_requests_this_period = true;
    }
}

void SwimProcess::handle_message() {
        auto [msg, sender_ip, sender_port] = m_server->receive_from();
        Message proto_msg;
        proto_msg.ParseFromString(msg);

        switch (proto_msg.type()) {
            case Message::PING: {
                std::cout << log_header() << "recieved PING with " << proto_msg.members_size() << " member updates" << std::endl;
                bool new_member = update_member_list(make_member(proto_msg.sender_id(), sender_port, Member::ALIVE));

                // update local memberlist with everything that was sent to us
                for (int i = 0; i < proto_msg.members_size(); ++i) {
                    update_member_list(proto_msg.members(i));
                }

                send_message(sender_ip, sender_port, Message::ACK, new_member);
                break;
            }
            case Message::ACK:
                if (proto_msg.sender_id() == m_unacknowledged_member.id()) {
                    std::cout << log_header() << "ping acknowledged" << std::endl;
                    m_unacknowledged_ping = false;
                    clear_member(m_unacknowledged_member);
                } else {
                    // TODO can't have this with indirect pings
                    std::cout << log_header(LOG_FATAL) << "unknown ACK" << std::endl;
                    std::exit(1);
                }
                break;
            default:
                std::cout << log_header(LOG_FATAL) << "Unknown message type" << std::endl;
                std::exit(1);
        }
}

void SwimProcess::send_ping_requests() {
    // can't send ping requests to self or to the member we are requesting
    size_t num_available = m_members.size() - 2;
    if (num_available <= 0) {
        std::cout << log_header() << "nobody to send ping requests to" << std::endl;
        return;
    }


    // the list is already randomly permuted, so just pick a "random" index and iterate
    // until we have sent all ping requests
    size_t index = std::rand() % m_id_list.size();
    size_t num_sent = 0;
    while (num_sent < num_available && num_sent < kNumIndirectPings) {
        if (index >= m_id_list.size()) {
            index = 0;
        }

        // can't send ping requests to self or to the member we are requesting
        if (m_id_list[index] == m_id || m_id_list[index] == m_unacknowledged_member.id()) {
            ++index;
        } else {
            auto& recipient = m_members.at(m_id_list[index]);
            std::cout << log_header() << "send ping req to " << recipient.port() << std::endl;
            send_message("127.0.0.1", recipient.port(), Message::PING_REQ);
            ++num_sent;
        }
    }
}

bool SwimProcess::send_ping() {
    static unsigned int ping_index = 0;

    // this process is the only member
    if (m_id_list.size() <= 1) {
        return false;
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

    auto& ping_recipient = m_members.at(m_id_list[ping_index]);
    m_unacknowledged_member = ping_recipient;

    std::cout << log_header() << "send ping to " << ping_recipient.id() << ":"
              << ping_recipient.port() << " (" << m_recent_updates.size()
              << " recent updates)" << std::endl;
    send_message("127.0.0.1", ping_recipient.port(), Message::PING);
    ++ping_index;
    return true;
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
            case Message::PING: {
                bool new_member = update_member_list(make_member(swim_msg.sender_id(), port, Member::ALIVE));
                send_message(ip, port, Message::ACK, new_member);
                break;
            }
            default:
                std::cout << log_header(LOG_FATAL) << "Received msg before any members have joined" << std::endl;
                std::exit(1);
        }
    }
}


void SwimProcess::send_message(const std::string& ip, unsigned int port, Message::Type msg_type, bool send_whole_list) {
    Message proto_msg;
    proto_msg.set_type(msg_type);
    proto_msg.set_sender_id(m_id);

    if (msg_type == Message::PING_REQ) {
        // TODO add unacknowledged member to the message
    }



    // we send the whole membership list to new members
    if (send_whole_list) {
        std::cout << log_header() << "send whole list (" << m_members.size() << " members) to new member" << std::endl;
        for (auto& m : m_members) {
            Member* new_member = proto_msg.add_members();
            new_member->set_status(m.second.status());
            new_member->set_id(m.first);
            new_member->set_port(m.second.port());
        }
    } else {    // only send recent updates
        for (auto& u : m_recent_updates) {
            Member* new_member = proto_msg.add_members();
            new_member->set_status(u.member.status());
            new_member->set_id(u.member.id());
            new_member->set_port(u.member.port());
            --u.retransmit;
        }
    }

    // remove any updates that are no longer recent
    while (m_recent_updates.front().retransmit <= 0) {
        m_recent_updates.pop_front();
    }

    std::string msg;
    proto_msg.SerializeToString(&msg);
    m_server->send(ip, port, msg);
}


bool SwimProcess::attempt_join() {
    std::cout << log_header() << "attempting join" << std::endl;
    for (size_t i = 0; i < kMaxJoinRetries; ++i, std::cout << log_header() << "re-attempting join" << std::endl) {

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


bool SwimProcess::update_member_list(const Member& member) {
    auto existing_member_itr = m_members.find(member.id());
    if (existing_member_itr != m_members.end()) {
        auto& existing_member = existing_member_itr->second;
        if (member.status() != existing_member.status()) {
            std::cout << log_header() << "updating member " << existing_member.id()
                      << ": " << status_to_str(existing_member.status())
                      << " --> " << status_to_str(member.status());
            existing_member.set_status(member.status());
        }
        return false;
    } else {    // new member
        m_members[member.id()] = member;
        m_id_list.push_back(member.id());
        m_recent_updates.push_back({member});

        std::cout << log_header() << "added member " << member.id() << ":" << member.port() << std::endl;
        return true;
    }
}

} // namespace swim
