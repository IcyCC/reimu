//
// Created by 苏畅 on 2019/6/21.
//

#ifndef REIMU_CHANNEL_H
#define REIMU_CHANNEL_H

#include "reimu_imp.h"


namespace reimu {
    typedef std::function<void()> ChannelCallBack;

    class Channel : public noncopyable {
    private:
        std::unique_ptr<Socket> _socket;
        EventLoop *_loop;
        short _listen_events;
        short _events;
        int _poll_index; // poll的下标 方便直接修改
        ChannelCallBack _read_cb, _write_cb;
    public:
        Channel(EventLoop *loop, int fd);

        Channel(EventLoop *loop);

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

        void OnWrite(const ChannelCallBack &cb) {
            _write_cb = cb;
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
        // 启用时间 传入是否启用 返回启用状态


        void EnableReadEvent(bool e);


        void EnableWriteEvent(bool e);

    };
}

#endif //REIMU_CHANNEL_H
