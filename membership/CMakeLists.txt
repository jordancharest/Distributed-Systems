CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(swim-membership-protocol)
add_library(platform STATIC
    ../platform/udp-server.cc
    ../platform/faulty-process.cc
)

# install(TARGETS platform DESTINATION ../../lib)

add_executable(swim-protocol
    swim-main.cc
    swim-process.cc
    msg/swim.pb.cc
)
set_property(TARGET swim-protocol PROPERTY CXX_STANDARD 20)
target_compile_options(swim-protocol PRIVATE -Wall -Wextra -Wpedantic -Werror -O2)

target_include_directories(swim-protocol
    PRIVATE
    ..
)

target_link_libraries(swim-protocol
    pthread
    platform
    protobuf
)