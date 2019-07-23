//
// Created by 苏畅 on 2019/7/23.
//
#ifndef  defined(OS_MACOSX)

#include "KQPoller.h"
#include "Channel.h"
#include <sys/event.h>

namespace reimu {

    KQPollerImp::KQPollerImp(Poller *poller) {
        _poller = poller;
        _fd = kqueue();
    }

    void KQPollerImp::AddChannel(reimu::Channel *ch) {
        UpdateChannel(ch);
    }

    void KQPollerImp::RemoveChannel(reimu::Channel *ch) {
        auto channel_item = _channels.find(ch->Fd());
        if (channel_item != _channels.end()) {
            _pollfds.erase(_pollfds.end() + ch->GetPollIndex());
            _channels.erase(ch->Fd());
        }
    }

    void KQPollerImp::UpdateChannel(reimu::Channel *ch) {
        auto channel_item = _channels.find(ch->Fd());
        if (channel_item == _channels.end()) {
            // 不存在新增
            _channels.insert(std::make_pair(ch->Fd(), ch));
            struct kevent event;
            if (ch->Readalbe()) {
                EV_SET(&event, ch->Fd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, ch);
            }
            if (ch->Writable()) {
                EV_SET(&event, ch->Fd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, ch);
            }

            _pollfds.push_back(event);
            int idx = _pollfds.size() - 1;
            ch->SetPollIndex(idx);
            kevent(_fd, &event, 1, NULL, 0, NULL);    // 添加
        } else {
            // 存在更新
            auto event = _pollfds[ch->GetPollIndex()];
            _pollfds[ch->GetPollIndex()] = event;
            if (ch->Readalbe()) {
                EV_SET(&event, ch->Fd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, ch);
            } else {
                EV_SET(&event, ch->Fd(), EVFILT_READ, EV_DELETE|EV_DISABLE, 0, 0, ch);
            }

            if (ch->Writable()) {
                EV_SET(&event, ch->Fd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, ch);
            } else {
                EV_SET(&event, ch->Fd(), EVFILT_WRITE, EV_DELETE|EV_DISABLE, 0, 0, ch);
            }

            kevent(_fd, &event, 1, NULL, 0, NULL);    // 添加
        }
    }

    void KQPollerImp::Poll(int waitMs, reimu::ChannelList &activateChannels) {
        struct timespec timeout;
        timeout.tv_sec = waitMs / 1000;
        timeout.tv_nsec = (waitMs % 1000) * 1000 * 1000;
        int numEvents = ::kevent(_fd, NULL, 0, &*_pollfds.begin(), _pollfds.size(), &timeout);
        for (auto pfd = _pollfds.begin();
             pfd != _pollfds.end() && numEvents > 0; ++pfd) {
            if (pfd->filter > 0) {
                numEvents--;
                auto channel_item = _channels.find(pfd->ident);

                if (channel_item == _channels.end()) {
                    // 异常
                    exit(1);
                }
                channel_item->second->ShortEvent(pfd->filter);
                activateChannels.push_back(channel_item->second);
            }

        }
    }
}

#endif
