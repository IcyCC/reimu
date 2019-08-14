//
// Created by 苏畅 on 2019/6/21.
//

#include "Channel.h"
#include "poller/Poller.h"
#include "event/EventLoop.h"

namespace reimu  {
    Channel::Channel(EventLoop *loop, SocketPtr socket) {
        _loop = loop;
        _socket = socket;
        _events = 0;
        _read_cb = _write_cb = nullptr;
    }

    Channel::Channel(reimu::EventLoop *loop) {
        _loop = loop;
        _socket = std::make_shared<Socket>();
        _events = 0;
        _read_cb = _write_cb = nullptr;
    }

    Channel::~Channel() {
        Close();
    }

    void Channel::update() {
        if (_socket != nullptr) {
            _loop->UpdateChannel(this);
        } else {
            exit(1);
        }
    }

    void Channel::Close() {
        _listen_events = 0;
        _socket->ShutdownWrite();
        _loop->RemoveChannel(this);
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

    void Channel::AttachSocket(SocketPtr socket) {
        _socket.swap(socket);
        this->_loop->AddChannel(this);
    }


    void Channel::CleanUp() {
        Close();
        _socket.reset();
    }

    bool Channel::Readalbe() {
        return _listen_events & this->_loop->GetPoller()->REIMU_POLLIN;
    }

    bool Channel::Writable() {
        return _listen_events & this->_loop->GetPoller()->REIMU_POLLOUT;
    }



}