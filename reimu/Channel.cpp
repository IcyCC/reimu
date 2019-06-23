//
// Created by 苏畅 on 2019/6/21.
//

#include "Channel.h"
#include "SocketFd.h"

namespace reimu  {
    Channel::Channel(reimu::EventLoop *loop, int fd) {
        _loop = loop;
        _socket = std::make_unique<Socket>(fd, DeafultLocaAddr);
        _events = 0;
        _read_cb = _write_cb = _err_cb = nullptr;
    }

    Channel::~Channel() {
        Close();
    }

    void Channel::update() {
        _loop->UpdateChannel(this);
    }

    void Channel::Close() {
        _loop->RemoveChannel(this);
    }
}