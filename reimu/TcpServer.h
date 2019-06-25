//
// Created by 苏畅 on 2019/6/25.
//

#ifndef REIMU_TCPSERVER_H
#define REIMU_TCPSERVER_H

#include "reimu_imp.h"
#include "Util.h"
#include "Acceptor.h"


namespace reimu {
    class TcpServer : public noncopyable {
    public:
        std::map<int, TcpConnPtr> _conn_map;
        std::unique_ptr<Acceptor> _acceptor;
        TcpCallBack _conn_connected_cb, _conn_disconnected_cb, _conn_timeout_cb; //这些回调会注册到每一个conn上
        TcpMsgCallBack _conn_msg_cb; //这些回调会注册到每一个conn上
        TcpCallBack _accept_cb; //暴露给用户
        EventLoop *_loop;
    public:
        TcpServer(EventLoop *loop, const IPv4Addr &addr);

        TcpServer(EventLoop *loop, const std::string &host, unsigned short port) : TcpServer(loop,
                                                                                             IPv4Addr(host, port)) {};

        void StartServer();
    public:
        void OnConnMsg(const TcpMsgCallBack &cb) { _conn_msg_cb = cb; }; // 收到消息
        void OnConnConnected(const TcpCallBack &cb) { _conn_connected_cb = cb; }; //链接建立
        void OnConnDisconnected(const TcpCallBack &cb) { _conn_disconnected_cb = cb; }; // 链接断开
        void OnConnTimeout(const TcpCallBack &cb) { _conn_timeout_cb = cb; }; // 超时

        void OnAccept(const TcpCallBack &cb) { _accept_cb = cb; };

    public:
        void handleAccept(SocketPtr socket);
    };

}


#endif //REIMU_TCPSERVER_H
