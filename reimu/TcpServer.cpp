//
// Created by 苏畅 on 2019/6/25.
//

#include "TcpServer.h"
#include "Channel.h"
#include "TcpConn.h"

namespace reimu {
    TcpServer::TcpServer(EventLoop *loop, const reimu::IPv4Addr &addr): _loop(loop) {
        _acceptor = std::make_unique<Acceptor>(_loop, addr);
        _acceptor->OnAccept([this](SocketPtr socket){
            this->handleAccept(socket);
        });
    }

    void TcpServer::StartServer() {
        _acceptor->Listen();
    }

    void TcpServer::handleAccept(reimu::SocketPtr socket) {
        auto c = std::make_shared<TcpConn>(_loop, -1, socket);
        c->OnMsg(_conn_msg_cb);
        c->OnConnected(_conn_connected_cb);
        c->OnDisconnected(_conn_disconnected_cb);
        c->OnTimeout(_conn_timeout_cb);
        c->SetConnState(TcpConn::TcpConnState::CONNECTED);
        _conn_map.insert(std::make_pair(c->Fd(), c));
        if (_accept_cb != nullptr) {
            _accept_cb(c);
        }
    }
}