//
// Created by 苏畅 on 2019/6/22.
//

#include "TcpConn.h"
#include "Channel.h"
#include "event/EventLoop.h"

namespace reimu {

    TcpConn::~TcpConn() {
        CleanUp();
    }

    void TcpConn::Connect(const reimu::IPv4Addr &addr) {
        _dest_addr = addr;
        _loop->AddChannel(_channel.get());
        int ret = ::connect(_channel->Fd(), (struct sockaddr *) &addr.addr_, sizeof(struct sockaddr));
        if (ret == -1) {

        }
        _state = TcpConnState ::SHAKEHANDS;

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
        auto s = _output_buf.ToSlice();
        while (sended < s.size()) {
            auto sended_s = _output_buf.ToSlice();
            size_t t = write(_channel->Fd(), s.data(), s.size());
            _output_buf.Consume(t);
            sended = sended + t;
        }
        return sended;
    }

    void TcpConn::initChannel() {

        // 初始化频道

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
        auto conn = shared_from_this();
        while (_state == TcpConn::CONNECTED) {
            char buffer[READ_BUFFER_SIZE];
            size_t n = readImp(_channel->Fd(), buffer, sizeof(buffer));
            if (n <= 0) {
                // 处理错误
                if (n == 0) {
                    // 套接字对端断开
                    if (_disconnected_cb != nullptr ){
                        _loop->CreateTask([this,conn](){this->_disconnected_cb(conn);});
                    }
                    break;
                } else if (n == -1 && errno == EINTR) {
                    continue;
                } else if (n == -1 && errno == (errno == EAGAIN || errno == EWOULDBLOCK)) {
                    // io未就绪
                    break;
                }
            } else {
                _input_buf.Write(buffer);
            }

            while (true){
                auto s = _input_buf.ToSlice();
                auto msg =_codec->tryDecode(s);
                if (!msg.empty()){
                    // 解析到数据 调用回调并且尝试再次解析
                    _loop->CreateTask([this, msg,conn](){
                        this->_msg_cb(conn, msg);
                    });
                    _input_buf.Consume(msg.size());
                    continue;
                } else {
                    return;
                }
            }
        }

    }

    void TcpConn::handleWrite() {
        if (_state == TcpConnState::CONNECTED ){
            auto s = _output_buf.ToSlice();
            size_t sended = writeImp(_channel->Fd(), s.data(), s.size());
            _output_buf.Consume(sended);
        } else if (_state == TcpConnState::SHAKEHANDS) {
            _state = TcpConnState::CONNECTED;
            auto conn = shared_from_this();
            _loop->CreateTask([this, conn](){
                if (_connected_cb != nullptr ){
                    this->_connected_cb(conn);
                }
            });
        }

    }

    void TcpConn::handleTimeout() {

    }

    void TcpConn::CleanUp() {
        this->_channel.reset(nullptr);
    }
}