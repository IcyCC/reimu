//
// Created by 苏畅 on 2019/7/23.
//

#ifndef REIMU_KQPOLLER_H
#define REIMU_KQPOLLER_H

#ifndef  defined(OS_MACOSX)
#include "reimu_imp.h"
#include "Poller.h"
#include <sys/event.h>


namespace reimu {
    class KQPollerImp : public PollerImpAbc {
    public:
        void AddChannel(Channel* ch) override;

        void RemoveChannel(Channel* ch) override;

        void UpdateChannel(Channel* ch) override;

        void Poll(int waitMs, ChannelList &activateChannels) override;

        KQPollerImp(Poller *poller);

    private:
        Poller * _poller;
        int _fd;
        std::map<int, Channel*> _channels;
        typedef std::vector<struct kevent> PoolEvents;
        PoolEvents _pollfds;
    };
}

#endif


#endif //REIMU_KQPOLLER_H
