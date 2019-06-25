//
// Created by 苏畅 on 2019/6/19.
//

#pragma once
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <memory>
#include <chrono>
#include <utility>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "Util.h"
#include <arpa/inet.h>
#include "thead/SafeCounter.h"
#include "thead/SafeQueue.h"
#include "Codec.h"
#include "SocketFd.h"


namespace reimu {
    class Poller;
    class Channel;
    class EventLoop;
    class Task;
    class Timer;
    class TcpConn;
    class Socket;
    class Acceptor;
    class TcpServer;

    typedef std::shared_ptr<Channel> ChannelPrt;
    typedef std::vector<Channel*> ChannelList;
    typedef std::function<void()> TaskCallBack;

    typedef std::shared_ptr<Task> TaskPtr;
    typedef std::shared_ptr<Timer> TimerPtr;
    typedef std::shared_ptr<Socket> SocketPtr;
    typedef std::shared_ptr<TcpConn> TcpConnPtr;

    typedef std::function<void(TcpConnPtr, Slice)> TcpMsgCallBack;
    typedef std::function<void(TcpConnPtr)> TcpCallBack;

    typedef std::function<void (SocketPtr)> AccpetCallBack;

    class ThreadingPool;

    extern SafeCounter REIMU_GLOBAL_COUNTER;

}
