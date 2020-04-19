CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

project(swim-membership-protocol)
add_library(platform STATIC
    ../platform/udp-server.cc
    ../platform/faulty-process.cc
)

# install(TARGETS platform DESTINATION ../../lib)

add_executable(swim
    swim-main.cc
)

target_include_directories(swim
    PRIVATE
    ..
)

target_link_libraries(swim
    pthread
    platform
)