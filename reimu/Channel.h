//
// Created by 苏畅 on 2019/6/21.
//

#ifndef REIMU_CHANNEL_H
#define REIMU_CHANNEL_H

#include "reimu_imp.h"
#include "SocketFd.h"
#include "event/EventLoop.h"

namespace reimu {
    typedef std::function<void()> ChannelCallBack;

    class Channel : public noncopyable {
    private:
        std::unique_ptr<Socket> _socket;
        EventLoop *_loop;
        short _listen_events;
        short _events;
        int _poll_index; // poll的下标 方便直接修改
        ChannelCallBack _read_cb, _write_cb, _err_cb;
    public:
        Channel(EventLoop *loop, int fd);

        ~Channel();

        void Close();

        EventLoop *GetEventLoop() {
            return _loop;
        }

        short GetEvents() {
            return _events;
        }

        void ShortEvent(short event) {
            _events = event;
        };

        short GetListenEvents() {
            return _listen_events;
        }

        int GetPollIndex() {
            return _poll_index;
        }

        void SetPollIndex(int poll_index) {
            _poll_index = poll_index;
        }

        int Fd() {
            return _socket->fd();
        }


        void update();


    public:

        // 用户注册事件的接口
        void OnRead(const ChannelCallBack &cb) {
            _read_cb = cb;
        };

        void OnRead(ChannelCallBack &&cb) {
            _read_cb = std::move(cb);
        };

        void OnWrite(const ChannelCallBack &cb) {
            _write_cb = cb;
        };

        void OnWrite(ChannelCallBack &&cb) {
            _write_cb = std::move(cb);
        };


        void OnError(const ChannelCallBack &cb) {
            _err_cb = cb;
        };

        void OnError(ChannelCallBack &&cb) {
            _err_cb = std::move(cb);
        };

        // Poller 调用
        void HandleRead() {
            if (_read_cb != nullptr) {
                _read_cb();
            }
        }

        void HandleWrite() {
            if (_write_cb != nullptr) {
                _write_cb();
            }
        }

        void HandleError() {
            if (_err_cb != nullptr) {
                _err_cb();
            }
        }

        // 启用时间 传入是否启用 返回启用状态


        void EnableReadEvent(bool e) {
            if (e) {
                _listen_events = _listen_events | _loop->GetPoller()->REIMU_POLLIN;
            } else {
                _listen_events = _listen_events & (_loop->GetPoller()->REIMU_POLLIN);
            }
            update();
        }


        void EnableWriteEvent(bool e) {
            if (e) {
                _listen_events = _listen_events | _loop->GetPoller()->REIMU_POLLOUT;
            } else {
                _listen_events = _listen_events & (~_loop->GetPoller()->REIMU_POLLOUT);
            }
            update();
        }


        void EnableErrorEvent(bool e) {
            if (e) {
                _listen_events = _listen_events | _loop->GetPoller()->REIMU_POLLERR;
            } else {
                _listen_events = _listen_events & (~_loop->GetPoller()->REIMU_POLLERR);
            }
            update();
        }
    };
}

#endif //REIMU_CHANNEL_H
