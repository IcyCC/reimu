cmake_minimum_required(VERSION 3.12)
project(reimu)

set(CMAKE_CXX_STANDARD 17)
include_directories(reimu)
add_executable(reimu
        reimu/SocketFd.cpp
        reimu/event/EventLoop.cpp
        reimu/Util.cpp
        reimu/thead/ThreadingPool.cpp
        reimu/Channel.cpp
        reimu/poller/Poller.cpp
        reimu/poller/PPoller.cpp

        reimu/Codec.cpp
        reimu/Buffer.cpp
        reimu/TcpConn.cpp

     reimu/Acceptor.cpp
        reimu/TcpServer.cpp

        reimu/log/Logging.cpp
        #tests/test_server_timeout.cpp
        tests/test_server.cpp
        #tests/test_timer.cpp
        # tests/test_threading.cpp
        #tests/test_buffer_slice.cpp
        #tests/test_conn.cpp
        )