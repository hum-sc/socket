cmake_minimum_required(VERSION 3.26)
project(socket)

set(CMAKE_CXX_STANDARD 17)

add_executable(socket
        src/servidor/server.cpp
        src/Socket.cpp
        src/Socket.h
        src/cliente/client.cpp
)

target_link_libraries(socket wsock32 ws2_32)