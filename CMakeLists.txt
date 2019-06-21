cmake_minimum_required(VERSION 3.12)
project(reimu)

set(CMAKE_CXX_STANDARD 17)

add_executable(reimu
        src/reimu_imp.h
        src/SocketFd.cpp
        src/SocketFd.h
        src/EventLoop.cpp
        src/EventLoop.h
        src/Util.cpp
        src/Util.h
        src/reimu.h
        tests/test_timer.cpp
        )