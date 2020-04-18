#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <unordered_map>

#include "faulty-process.hpp"
#include "udp-server.hpp"

int main(int argc, char** argv) {
    std::shared_ptr<platform::UdpServer> server = std::make_shared<platform::UdpServer>("faulty-process", 9999);

    // encapsulate the entire program in a FaultyProcess
    // so we can control crashes, message loss, etc.
    while (true) {
        std::cout << "Main: starting process" << std::endl;
        platform::FaultyProcess fp(server);
        std::thread process_thread([&](){ fp.run(); });

        int i = 0;
        while (true) {
            ++i;
            std::cout << "Main: generating random number " << i << std::endl;
            // sit and generate random numbers
            if (i == 10) {
                std::cout << "Main: crashing process" << std::endl;
                fp.crash();
                process_thread.join();
                break;
            }
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1s);
        }
    }

    return EXIT_SUCCESS;
}