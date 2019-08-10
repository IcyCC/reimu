//
// Created by 苏畅 on 2019/6/25.
//

#include "TcpServer.h"
#include "Channel.h"
#include "TcpConn.h"

namespace reimu {
    TcpServer::TcpServer(EventLoop *loop, const reimu::IPv4Addr &addr): _loop(loop) {
        _conn_timeout = -1;
        _acceptor = std::make_unique<Acceptor>(_loop, addr);
        _acceptor->OnAccept([this](SocketPtr socket){
            this->handleAccept(socket);
        });
    }

    void TcpServer::StartServer() {
        _acceptor->Listen();
        INFO_LOG.Log("服务器开始运行 监听: %s",  _acceptor->_listen_addr.ToString().c_str());
    }

    void TcpServer::handleAccept(reimu::SocketPtr socket) {
        INFO_LOG.Log("服务器收到 来自: %s 的请求", socket->_addr.ToString().c_str());
        auto c = std::make_shared<TcpConn>(_loop, _conn_timeout, socket);
        c->OnMsg(_conn_msg_cb);
        c->OnConnected(_conn_connected_cb);
        c->OnDisconnected([this] (reimu::TcpConnPtr conn){
            this->handleDisconnected(conn);
        });
        c->OnTimeout([this ](reimu::TcpConnPtr conn){
            this->handleTimeout(conn);
        });
        c->SetConnState(TcpConn::TcpConnState::CONNECTED);
        _conn_map.insert(std::make_pair(c->Fd(), c));
        if (_accept_cb != nullptr) {
            _accept_cb(c);
        }
    }

    void TcpServer::handleTimeout(reimu::TcpConnPtr conn) {
        INFO_LOG.Log("踢掉链接");
        if (_conn_timeout_cb != nullptr){
            _conn_timeout_cb(conn);
        }
        if(_conn_map.find(conn->Fd()) != _conn_map.end()) {
            _conn_map.erase(conn->Fd());
        }
    }

    void TcpServer::handleDisconnected(reimu::TcpConnPtr conn) {
        if (_conn_disconnected_cb != nullptr){
            _conn_disconnected_cb(conn);
        }
        if(_conn_map.find(conn->Fd()) != _conn_map.end()) {
            _conn_map.erase(conn->Fd());
        }
    }

    void TcpServer::SetConnTimeout(int timeout) {
        _conn_timeout = timeout;
        for(auto &i : _conn_map ){
            i.second->SetTimeout(timeout);
        }
    }
}