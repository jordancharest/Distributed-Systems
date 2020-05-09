#include <deque>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>

#include "msg/swim.pb.h"
#include "platform/faulty-process.hpp"
#include "platform/udp-server.hpp"



namespace swim {
class SwimProcess : public ::platform::FaultyProcess {
public:
    SwimProcess(std::shared_ptr<platform::UdpServer> server, unsigned int id,
                bool is_coordinator, std::string coordinator_host, int coordinator_port) :
        FaultyProcess(server),
        m_id(id),
        m_is_coordinator(is_coordinator),
        m_coord_host(std::move(coordinator_host)),
        m_coord_port(coordinator_port)
        {}

    /** Copying is forbidden, std::threads can't be copied */
    SwimProcess(const SwimProcess&) = delete;
    SwimProcess& operator=(const SwimProcess&) = delete;

    /** initialize and wrap the SWIM algorithm, follow the template for FaultyProcess */
    void run() override;

private:
    // execute a single loop of the SWIM algorithm
    void execute_swim();

    // coordinator: wait for first join request
    // other: send join request to coordinator
    void initialize();

    // Send a join message of type msg_type to the recipient described by the ip and port
    void send_message(const std::string& ip, unsigned int port, Message::Type msg_type);
    void handle_message();
    void send_ping();

    // Attempt to join for some number of retries
    bool attempt_join();

    void add_member(unsigned int id, unsigned int port);

    enum LogLevel {
        LOG_DEBUG = 0,
        LOG_INFO,
        LOG_FATAL
    };

    // log some information according to the log level
    std::string log_header(LogLevel lvl = LOG_INFO);



    const unsigned int m_id;
    const bool m_is_coordinator;
    std::string m_coord_host;
    unsigned int m_coord_port;

    std::unordered_map<unsigned int, Member> m_members;

    struct MemberUpdate {
        Member member;
        int retransmit = 3;
    };
    std::deque<MemberUpdate> m_recent_updates;
    std::vector<unsigned int> m_id_list;
};

} // namespace swim