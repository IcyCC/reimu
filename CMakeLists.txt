cmake_minimum_required(VERSION 3.12)
project(reimu)

set(CMAKE_CXX_STANDARD 17)

add_executable(reimu
        reimu/reimu_imp.h
        reimu/SocketFd.cpp
        reimu/SocketFd.h
        reimu/event/EventLoop.cpp
        reimu/event/EventLoop.h
        reimu/Util.cpp
        reimu/Util.h
        reimu/reimu.h
        reimu/thead/SafeQueue.h
        reimu/thead/ThreadingPool.cpp
        reimu/thead/ThreadingPool.h
        reimu/thead/SafeCounter.h
        reimu/event/Task.h

        reimu/Channel.cpp
        reimu/Channel.h
        reimu/poller/Poller.cpp
        reimu/poller/Poller.h
        reimu/poller/PPoller.cpp
        reimu/poller/PPoller.h

        reimu/Slice.h
        reimu/Codec.cpp
        reimu/Codec.h
        reimu/Buffer.cpp
        reimu/Buffer.h
        reimu/TcpConn.cpp
        reimu/TcpConn.h
        #tests/test_timer.cpp
        tests/test_threading.cpp
#        tests/test_buffer_slice.cpp
        )