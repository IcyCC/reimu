#include"Acceptor.h"
#include "event/EventLoop.h"
#include "Channel.h"


namespace reimu {
    Acceptor::Acceptor(EventLoop * loop, const IPv4Addr & lAddr): _loop(loop), _listen_addr(lAddr) {
        _channel = std::make_unique<Channel>(_loop);
        _channel->GetSocket()->BindAddress(_listen_addr);
        _channel->GetSocket()->SetReuseAddr(true);
        _channel->EnableReadEvent(true);
        _channel->OnRead([this](){
            this->HandleRead();
        });
    }

    void Acceptor::Listen() {
        _channel->GetSocket()->Listen();
        _loop->AddChannel(_channel.get());
    }

    void Acceptor::HandleRead() {
        IPv4Addr c_addr;
        int conn_fd = _channel->GetSocket()->Accept(&c_addr);
        if (conn_fd < 0 ){
            if ( (errno == EAGAIN) || (errno == EMFILE) || (errno == EINTR) || (errno == ECONNABORTED) ) {
                return;
            } else {
                exit(1);
            }
        } else {
            auto conn_socket  = std::make_shared<Socket>(conn_fd, c_addr);
            if (_accept_cb != nullptr) {
                _accept_cb(conn_socket);
            }
        }
    }

    void Acceptor::Close() {
        _channel->Close();
    }

    Acceptor::~Acceptor() {
    }

    void Acceptor::Cleanup() {
        _channel.reset(nullptr);
    }


}