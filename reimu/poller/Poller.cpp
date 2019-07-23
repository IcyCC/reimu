//
// Created by 苏畅 on 2019/6/21.
//

#include "Poller.h"
#include "PPoller.h"
#include "KQPoller.h"

namespace reimu {

    Poller::Poller() {
#if  defined(USE_EPOLL) && defined(OS_LINUX)
    // EPOLL
#elif USE_KQUEUE
        _imp = std::make_unique<PPollerImp>(this);
        REIMU_POLLIN = EVFILT_READ;
        REIMU_POLLOUT = EVFILT_WRITE;
        REIMU_POLLERR = EVFILT_EXCEPT;
#else
        _imp = std::make_unique<PPollerImp>(this);
        REIMU_POLLIN = POLLIN | POLLPRI;
        REIMU_POLLOUT = POLLOUT;
        REIMU_POLLERR = POLLERR;

#endif
    }

}