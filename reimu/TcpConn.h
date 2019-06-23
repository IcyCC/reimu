//
// Created by 苏畅 on 2019/6/22.
//

#ifndef REIMU_TCPCONN_H
#define REIMU_TCPCONN_H

#include "reimu_imp.h"

namespace reimu {
    class TcpConn : public std::enable_shared_from_this<TcpConn>, private noncopyable {
    private:
        std::unique_ptr<Channel> _channel;
        std::unique_ptr<CodecBase> _codec;
        EventLoop *_loop;
        TcpCallBack _connected_cb, _disconnected_cb, _timeout_cb;
        TcpMsgCallBack _msg_cb;
        Buffer _input_buf, _output_buf; // 读写缓存
        int _timeout;
    public:
        TcpConn(EventLoop *loop, int timeout) : _loop(loop), _timeout(timeout) {};

        TcpConn(EventLoop *loop, int timeout, int fd) : TcpConn(loop, timeout) {
            _channel = std::make_unique<Channel>(_loop, fd);
        }

    public:
        void OnMsg(const TcpMsgCallBack &cb) { _msg_cb = cb; }; // 收到消息
        void OnConnected(const TcpCallBack &cb) { _connected_cb = cb; }; //链接建立
        void OnDisconnected(const TcpCallBack &cb) { _disconnected_cb = cb; }; // 链接断开
        void OnTimeout(const TcpCallBack &cb) { _timeout_cb = cb; }; // 超时

    public:
        void Connect(const IPv4Addr &addr);//链接
        void Connect(const std::string &host, unsigned short port) {
            Connect(IPv4Addr(host, port));
        }
        void Reconnect();
        int Send(const std::string &s);

    public:
        void handleWrite();
        void handleRead();
        void handleConnect();
        void handleDisconnect();
        void handleTimeout();

        void clean();
    };
}


#endif //REIMU_TCPCONN_H
