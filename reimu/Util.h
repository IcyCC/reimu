//
// Created by 苏畅 on 2019/6/19.
//

#ifndef REIMU_UTIL_H
#define REIMU_UTIL_H

#include <chrono>
#include <cstdio>
#include <time.h>
#include <string>
#include <signal.h>
#include <functional>
#include <map>

namespace reimu {

    class noncopyable {
    protected:
        noncopyable() = default;

        virtual ~noncopyable() = default;

        noncopyable(const noncopyable &) = delete;

        noncopyable &operator=(const noncopyable &) = delete;
    };

    struct util {
        static int64_t TimeMicro();

        inline static int64_t TimeMilli() { return TimeMicro() / 1000; }

        inline static std::string CurrentReadableTime() {
            time_t timep;
            time(&timep);
            char tmp[64];
            strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
            return tmp;
        }
    };

    struct Signal {
        static std::map<int, std::function<void()>> handlers;
        static void signal(int sig, const std::function<void()> &handler);

        static inline void signal_handler(int sig) {
            handlers[sig]();
        }
    };
}


#endif //REIMU_UTIL_H
