cmake_minimum_required(VERSION 3.12)
project(reimu)

set(CMAKE_CXX_STANDARD 17)

add_executable(reimu
        src/reimu_imp.h
        src/SocketFd.cpp
        src/SocketFd.h
        src/event/EventLoop.cpp
        src/event/EventLoop.h
        src/Util.cpp
        src/Util.h
        src/reimu.h
        src/thead/SafeQueue.h
        src/thead/ThreadingPool.cpp
        src/thead/ThreadingPool.h
        src/thead/SafeCounter.h
        src/event/Task.h
        #tests/test_timer.cpp
        tests/test_threading.cpp
        src/Channel.cpp src/Channel.h)