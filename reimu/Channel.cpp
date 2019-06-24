//
// Created by 苏畅 on 2019/6/21.
//

#include "Channel.h"
#include "poller/Poller.h"
#include "event/EventLoop.h"

namespace reimu  {
    Channel::Channel(reimu::EventLoop *loop, int fd) {
        _loop = loop;
        _socket = std::make_unique<Socket>(fd, DeafultLocaAddr);
        _events = 0;
        _read_cb = _write_cb = nullptr;
    }

    Channel::Channel(reimu::EventLoop *loop) {
        _loop = loop;
        _socket = std::make_unique<Socket>();
        _events = 0;
        _read_cb = _write_cb = nullptr;
    }

    Channel::~Channel() {
        Close();
        _loop->RemoveChannel(this);
    }

    void Channel::update() {
        _loop->UpdateChannel(this);
    }

    void Channel::Close() {
        _listen_events = 0;
        _socket->ShutdownWrite();
        update();
    }

    void Channel::EnableReadEvent(bool e) {
        {
            if (e) {
                _listen_events = _listen_events | _loop->GetPoller()->REIMU_POLLIN;
            } else {
                _listen_events = _listen_events & (_loop->GetPoller()->REIMU_POLLIN);
            }
            update();
        }
    }

    void Channel::EnableWriteEvent(bool e)  {
        if (e) {
            _listen_events = _listen_events | _loop->GetPoller()->REIMU_POLLOUT;
        } else {
            _listen_events = _listen_events & (~_loop->GetPoller()->REIMU_POLLOUT);
        }
        update();
    }
}