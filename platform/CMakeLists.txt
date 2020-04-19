CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

project(distributed-system)
add_library(platform STATIC
    udp-server.cc
    faulty-process.cc
    network-layer.cc
)

install(TARGETS platform DESTINATION ../../lib)

add_executable(test-platform
    test-platform.cc
)

target_link_libraries(test-platform
    pthread
    platform
)