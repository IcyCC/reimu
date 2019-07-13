//
// Created by 苏畅 on 2019/6/21.
//

#ifndef REIMU_PPOLLER_H
#define REIMU_PPOLLER_H

#include "reimu_imp.h"
#include "Poller.h"
#include <sys/poll.h>

namespace reimu {
    class PPollerImp : public PollerImpAbc {
    public:
        void AddChannel(Channel* ch) override;

        void RemoveChannel(Channel* ch) override;

        void UpdateChannel(Channel* ch) override;

        void Poll(int waitMs, ChannelList &activateChannels) override;

        PPollerImp(Poller *poller);

    private:
        Poller * _poller;
        std::map<int, Channel*> _channels;
        typedef std::vector<struct pollfd> PollFdList;
        PollFdList _pollfds;
    };
}


#endif //REIMU_PPOLLER_H
