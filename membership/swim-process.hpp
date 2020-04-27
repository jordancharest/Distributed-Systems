#include <platform/faulty-process.hpp>
#include <platform/udp-server.hpp>

namespace swim {
class SwimProcess : public platform::FaultyProcess {
public:
    SwimProcess(std::shared_ptr<platform::UdpServer> server, bool is_coordinator,
                std::string coordinator_host, int coordinator_port) :
    FaultyProcess(server),
    m_is_coordinator(is_coordinator),
    m_coord_host(std::move(coordinator_host)),
    m_coord_port(coordinator_port) 
    {}

    /** Copying is forbidden, std::threads can't be copied */
    SwimProcess(const SwimProcess&) = delete;
    SwimProcess& operator=(const SwimProcess&) = delete;

    /** Implement the SWIM algorithm, follow the template for FaultyProcess */
    void run() override;

private:
    bool m_is_coordinator;
    std::string m_coord_host;
    int m_coord_port;
};

} // namespace swim