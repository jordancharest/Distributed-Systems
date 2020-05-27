#include <iostream>
#include <limits>
#include <random>
#include <thread>
#include <getopt.h>

#include "platform/faulty-process.hpp"
#include "platform/udp-server.hpp"
#include "swim-process.hpp"
#include "msg/swim.pb.h"



std::tuple<std::string, int, std::string, int, bool> parse_args(int argc, char** argv) {
    std::string host = "";
    int port = 0;
    std::string chost = "";
    int cport = 0;
    int coordinator = 0;

    while (true) {
        static struct option long_options[] = 
        {
            {"coordinator", no_argument,       &coordinator,   1},
            {"host",        required_argument, 0,            'h'},
            {"port",        required_argument, 0,            'p'},
            {"chost",       required_argument, 0,            's'},
            {"cport",       required_argument, 0,            'r'},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        int c = getopt_long(argc, argv, "", long_options, &option_index);
        
        // options parsing is done
        if (c == -1) {
            break;
        }

        switch (c) {
            case 0: // a long option was found
                std::cout << "Option " << long_options[option_index].name << "\n";
                break;
            case 'h':
                host = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 's':
                chost = optarg;
                break;
            case 'r':
                cport = atoi(optarg);
                break;
            default:    // unrecognized option
                std::exit(1);
                break;
        }
    }

    if (optind < argc) {
        std::cout << "Non-option args:";
        while (optind < argc) {
            std::cout << " " << argv[optind++];
        }
        std::cout << "\n";
    }

    if (host.empty() || !port) {
        std::cerr << "hostname and port are required\n";
        std::exit(1);
    }

    return {host, port, chost, cport, coordinator};
}


int main (int argc, char** argv) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    auto [host, port, chost, cport, is_coordinator] = parse_args(argc, argv);
    if (is_coordinator) {
        std::cout << "MAIN: this faulty process will be the coordinator\n";
    } else if (chost.empty() || !cport) {
        std::cerr << "Process needs to either be the coordinator (--coordinator)"
                  << " or know how to contact coordinator through chost"
                  << " and cport" << std::endl;
        std::exit(1);
    }

    std::cout << "MAIN: starting server " << host << ":" << port << "\n";
    std::shared_ptr<platform::UdpServer> server = std::make_shared<platform::UdpServer>(host, port);

    // to generate thread ids
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::uniform_int_distribution<uint64_t> dist(1, std::numeric_limits<uint64_t>::max());

    // this process acts as a watchdog for the faulty process. When the faulty process crashes,
    // it is automatically restarted
    while (true) {
        // encapsulate the entire program in a FaultyProcess
        // so we can control crashes, message loss, etc.
        swim::SwimProcess sp(server, dist(generator), is_coordinator, chost, cport);
        std::thread process_thread([&](){ sp.run(); });

        int i = 0;
        while (true) {
            ++i;

            // sit and generate random numbers, don't crash the coordinator for now
            if (!is_coordinator && i == 60) {
                std::cout << "MAIN: crashing process" << std::endl;
                sp.crash();
                process_thread.join();
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // FOR DEBUGGING
        break;
    }
    std::cout << "MAIN: exiting" << std::endl;

    google::protobuf::ShutdownProtobufLibrary();
    return EXIT_SUCCESS;
}