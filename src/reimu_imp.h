//
// Created by 苏畅 on 2019/6/19.
//

#pragma once
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <cstdio>
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

namespace reimu {
    class EventLoop;

    class Task;
    class Timer;

    class Channel;

    typedef std::function<void()> TaskCallBack;
    typedef std::shared_ptr<Task> TaskPtr;
    typedef std::shared_ptr<Timer> TimerPtr;

    class ThreadingPool;

    extern SafeCounter REIMU_GLOBAL_COUNTER;

}
