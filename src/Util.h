//
// Created by 苏畅 on 2019/6/19.
//

#ifndef REIMU_UTIL_H
#define REIMU_UTIL_H

#include <chrono>
#include <cstdio>

namespace reimu {

    struct noncopyable {
    protected:
        noncopyable() = default;

        virtual ~noncopyable() = default;

        noncopyable(const noncopyable &) = delete;

        noncopyable &operator=(const noncopyable &) = delete;
    };

    struct util {
        static int64_t TimeMicro();
        inline static int64_t TimeMilli() { return TimeMicro() / 1000; }
    };
}


#endif //REIMU_UTIL_H
