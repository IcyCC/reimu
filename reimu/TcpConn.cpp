//
// Created by 苏畅 on 2019/6/22.
//

#include "TcpConn.h"
#include "Channel.h"
#include "event/Task.h"
#include "event/EventLoop.h"

namespace reimu {

    TcpConn::~TcpConn() {
    }

    void TcpConn::Connect(const reimu::IPv4Addr &addr) {
        _dest_addr = addr;
        refreshTimeoutTimer();
        int ret = ::connect(_channel->Fd(), (struct sockaddr *) &addr.addr_, sizeof(struct sockaddr));
        if (ret == -1) {

        }
        _state = TcpConnState::SHAKEHANDS;

    }


    void TcpConn::Reconnect() {
        if (_state != CONNECTED) {
            Connect(_dest_addr);
        }
    }

    void TcpConn::Send(const std::string &s) {
        _codec->encode(s, _output_buf);
    }

    void TcpConn::Close() {
        bSend();
        _channel->Close();
        _state = TcpConnState::CLOSED;
    }

    size_t TcpConn::bSend() {
        size_t sended = 0;
        auto s = _output_buf->ToSlice();
        while (sended < s.size()) {
            size_t t = write(_channel->Fd(), s.data(), s.size());
            _output_buf->Consume(t);
            sended = sended + t;
        }
        return sended;
    }

    void TcpConn::initChannel() {

        // 初始化频道
        _loop->AddChannel(_channel.get());
        _channel->OnRead([this]() {
            this->handleRead();
        });

        _channel->OnWrite([this]() {
            this->handleWrite();
        });

        _channel->EnableWriteEvent(true);
        _channel->EnableReadEvent(true);
    }

    void TcpConn::handleRead() {
        refreshTimeoutTimer();
        while (_state == TcpConn::CONNECTED) {
            auto conn = shared_from_this();
            char buffer[READ_BUFFER_SIZE];
            bzero(buffer, READ_BUFFER_SIZE);
            size_t n = readImp(_channel->Fd(), buffer, sizeof(buffer));
            if (n <= 0) {
                // 处理错误
                if (n == 0) {
                    // 套接字对端断开
                    _state = TcpConnState::CLOSED;
                    if (_disconnected_cb != nullptr) {
                        _loop->CreateTask([this, conn]() { this->_disconnected_cb(conn); });
                    }
                    break;
                } else if (n == -1 && errno == EINTR) {
                    continue;
                } else if (n == -1 && errno == (errno == EAGAIN || errno == EWOULDBLOCK)) {
                    // io未就绪
                    break;
                }
            } else {
                _input_buf->Write(buffer);
            }

            auto s = _input_buf->ToSlice();
            auto msg = _codec->tryDecode(s);
            if (!msg.empty()) {
                _input_buf->Consume(msg.size());
                _loop->CreateTask([this, msg, conn]() {
                    this->_msg_cb(conn, msg);
                });
            }
            return;
        }

    }

    void TcpConn::handleWrite() {
        if (_state == TcpConnState::CONNECTED) {
            auto s = _output_buf->ToSlice();
            if (!s.empty()) {
                refreshTimeoutTimer();
                size_t sended = writeImp(_channel->Fd(), s.data(), s.size());
                _output_buf->Consume(sended);
            }
        } else if (_state == TcpConnState::SHAKEHANDS) {
            refreshTimeoutTimer();
            _state = TcpConnState::CONNECTED;
            auto conn = shared_from_this();
            _loop->CreateTask([this, conn]() {
                if (_connected_cb != nullptr) {
                    this->_connected_cb(conn);
                }
            });
        }

    }

    void TcpConn::handleTimeout() {
        _state = TcpConnState ::ERROR;
        auto conn = shared_from_this();
        this->_timeout_timer->Cancel();
        this->_timeout_timer.reset();
        if (_timeout_cb != nullptr){
            _timeout_cb(conn);
            CleanUp();
        }
    }

    void TcpConn::CleanUp() {
        _state = TcpConnState::CLOSED;
        if (_timeout_timer != nullptr){
            _timeout_timer->Cancel();
            _timeout_timer.reset();
        }
        this->_channel.reset(nullptr);
    }

    void TcpConn::AttachChannel(reimu::Channel *channel, IPv4Addr &rAddr) {
        _channel.reset(channel);
        _dest_addr = rAddr;
        initChannel();
    }

    int TcpConn::Fd() {
        return _channel->Fd();
    }

    void TcpConn::refreshTimeoutTimer() {
            if (_timeout_timer != nullptr){
                _timeout_timer->Cancel();
            }
            if (_timeout > 0){
                _timeout_timer = _loop->CallLater([this](){
                    handleTimeout();
                }, _timeout);
            }
    }

    Channel *TcpConn::GetChannel() {
        return _channel.get();
    }


}
