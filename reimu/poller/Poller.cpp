//
// Created by 苏畅 on 2019/6/21.
//

#include "Poller.h"
#include "PPoller.h"


namespace reimu {

    Poller::Poller() {
#ifdef USE_EPOLL
    // EPOLL
#else
        _imp = std::make_unique<PPollerImp>(this);
        REIMU_POLLIN = POLLIN | POLLPRI;
        REIMU_POLLOUT = POLLOUT;
        REIMU_POLLERR = POLLERR;

#endif
    }

}