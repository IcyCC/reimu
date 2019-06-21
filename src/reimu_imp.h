//
// Created by 苏畅 on 2019/6/19.
//

#ifndef REIMU_REIMU_IMP_H
#define REIMU_REIMU_IMP_H

#include <sys/socket.h>
#include <sys/fcntl.h>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <chrono>
#include <utility>
#include <functional>
#include "Util.h"

namespace reimu {
    class EventLoop;

    class Task;
    class Timer;
    typedef std::function<void()> TaskCallBack;

    typedef std::shared_ptr<Task> TaskPtr;
    typedef std::shared_ptr<Timer> TimerPtr;

    class Timer;

}

#endif //REIMU_REIMU_IMP_H
