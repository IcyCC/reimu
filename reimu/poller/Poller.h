//
// Created by 苏畅 on 2019/6/21.
//

#ifndef REIMU_POLLER_H
#define REIMU_POLLER_H

#include "../reimu_imp.h"

namespace reimu {

    class PollerImpAbc : public noncopyable {

    public:
        virtual  void AddChannel(Channel* ch) = 0;

        virtual void RemoveChannel(Channel* ch) = 0;

        virtual void UpdateChannel(Channel* ch) = 0;

        virtual void Poll(int waitMs, ChannelList &activateChannels) = 0;

    };


    class Poller : public noncopyable {

    public:
        short REIMU_POLLIN;
        short REIMU_POLLOUT;
        short REIMU_POLLERR;

    public:

        void AddChannel(Channel* ch) {
            return _imp->AddChannel(ch);
        };

        void RemoveChannel(Channel* ch) {
            return _imp->RemoveChannel(ch);
        };

        void UpdateChannel(Channel* ch) {
            return _imp->UpdateChannel(ch);
        }

        void Poll(int waitMs, ChannelList &activateChannels) {
            return _imp->Poll(waitMs, activateChannels);
        }

        Poller();

    private:
        std::unique_ptr<PollerImpAbc> _imp;
    };
}


#endif //REIMU_POLLER_H
