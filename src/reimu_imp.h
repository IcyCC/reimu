//
// Created by 苏畅 on 2019/6/19.
//

#ifndef REIMU_REIMU_H
#define REIMU_REIMU_H

#include <sys/socket.h>
#include <sys/fcntl.h>
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
    typedef std::shared_ptr<Task> TaskPtr;
    typedef std::shared_ptr<Timer> TimerPtr;


}

#endif //REIMU_REIMU_H
