//
// Created by 苏畅 on 2019/6/21.
//

#include "PPoller.h"
#include "Channel.h"

namespace reimu {
    void PPollerImp::AddChannel(Channel* ch) {
        UpdateChannel(ch);
    }

    void PPollerImp::RemoveChannel(Channel* ch) {
        auto channel_item = _channels.find(ch->Fd());
        if (channel_item != _channels.end()) {
            _pollfds.erase(_pollfds.begin() + ch->GetPollIndex());
            _channels.erase(ch->Fd());
        }
    }

    void PPollerImp::UpdateChannel(Channel* ch) {
        auto channel_item = _channels.find(ch->Fd());
        if (channel_item == _channels.end()) {
            // 不存在新增
            _channels.insert(std::make_pair(ch->Fd(), ch));
            struct pollfd pfd;
            pfd.fd = ch->Fd();
            pfd.events = ch->GetListenEvents();
            pfd.revents = 0;
            _pollfds.push_back(pfd);
            int idx = _pollfds.size() - 1;
            ch->SetPollIndex(idx);
        } else {
            // 存在更新
            auto pfd = _pollfds[ch->GetPollIndex()];
            pfd.revents = 0;
            pfd.events = ch->GetListenEvents();
            pfd.fd = ch->Fd();
            _pollfds[ch->GetPollIndex()] = pfd;
        }
    }

    PPollerImp::PPollerImp(reimu::Poller *poller) {
        _poller = poller;
    }

    void PPollerImp::Poll(int waitMs, ChannelList &activateChannels) {
        int numEvents = ::poll(&*_pollfds.begin(), _pollfds.size(), waitMs);
        for (auto pfd = _pollfds.begin();
             pfd != _pollfds.end() && numEvents > 0; ++pfd) {
            if (pfd->revents > 0) {
                numEvents--;
                auto channel_item = _channels.find(pfd->fd);

                if (channel_item == _channels.end()) {
                    // 异常
                    exit(1);
                }
                channel_item->second->ShortEvent(pfd->revents);
                activateChannels.push_back(channel_item->second);
            }

        }
    }

}