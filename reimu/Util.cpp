//
// Created by 苏畅 on 2019/6/19.
//

#include "Util.h"

namespace reimu {
    int64_t util::TimeMicro() {
        std::chrono::time_point<std::chrono::system_clock> p = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(p.time_since_epoch()).count();
    };

    std::map<int, std::function<void()>> Signal::handlers;

    void Signal::signal(int sig, const std::function<void()> &handler) {
        handlers[sig] = handler;
        ::signal(sig, signal_handler);
    }

}