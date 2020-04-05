//
// Created by 苏畅 on 2019/6/22.
//

#ifndef REIMU_TCPCONN_H
#define REIMU_TCPCONN_H

#include "reimu_imp.h"

namespace reimu {
    struct AutoContext : noncopyable {
    void *ctx;
    std::function<void()> ctxDel;
    AutoContext() : ctx(0) {}
        template <class T>
        T &context() {
            if (ctx == NULL) {
                ctx = new T();
                ctxDel = [this] { delete (T *) ctx; };
            }
            return *(T *) ctx;
        }
        ~AutoContext() {
            if (ctx)
                ctxDel();
        }
    };


    class TcpConn : public std::enable_shared_from_this<TcpConn>, private noncopyable {
    public:
        enum TcpConnState {
            IDLE,
            SHAKEHANDS,
            CONNECTED,
            CLOSED,
            ERROR
        };
    private:

        const size_t READ_BUFFER_SIZE = 1024;

        std::unique_ptr<Channel> _channel;
        std::unique_ptr<CodecBase> _codec;
        EventLoop *_loop;
        TcpCallBack _connected_cb, _disconnected_cb, _timeout_cb;
        TcpMsgCallBack _msg_cb;
        BufferPtr _input_buf, _output_buf; // 读写缓存
        int _timeout;
        TcpConnState _state;
        IPv4Addr _dest_addr;

        TimerPtr _timeout_timer;

        std::mutex _mutex;

    public:
        AutoContext ctx_, internalCtx_;

    public:

        TcpConn(EventLoop *loop, int timeout) : _loop(loop), _timeout(timeout) {
            _channel = std::make_unique<Channel>(_loop);
            _state = TcpConnState::IDLE;
            _codec = std::make_unique<LineCodec>();
            _input_buf = std::make_shared<Buffer>();
            _output_buf = std::make_shared<Buffer>();
            initChannel();
        };

        TcpConn(EventLoop *loop, int timeout, SocketPtr socket) : _loop(loop), _timeout(timeout) {
            _channel = std::make_unique<Channel>(_loop, socket);
            _state = TcpConnState::IDLE;
            _input_buf = std::make_shared<Buffer>();
            _output_buf = std::make_shared<Buffer>();
            _codec = std::make_unique<LineCodec>();
            initChannel();
            refreshTimeoutTimer();
        }

        ~TcpConn();

        EventLoop *GetEventLoop() {
            return _loop;
        }

        void SetCodec(CodecBase *codec) {
            _codec.reset(codec);
        }

        int Fd();

        Channel *GetChannel();

        IPv4Addr GetDestAddr() {
            return _dest_addr;
        };

        TcpConnState GetState () {
            return _state;
        }

        void SetConnState(TcpConnState state) {
            _state = state;
        }

        void SetTimeout(int timeout) {
            _timeout = timeout;
        }

        int GetTimeout(){
            return _timeout;
        }

        void refreshTimeoutTimer();


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

        void Close();

        void CleanUp();


        void Send(const std::string &s);

        void AttachChannel(reimu::Channel *channel, IPv4Addr &rAddr);

    public:
        void handleWrite();

        void handleRead();


        void handleTimeout();

        void initChannel();

        size_t bSend(); // 阻塞发送完buffer所有数据

        size_t readImp(int fd, void *buf, size_t bytes) { return ::read(fd, buf, bytes); };

        size_t writeImp(int fd, const void *buf, size_t bytes) { return ::write(fd, buf, bytes); };

        template <class T>
        T &context() {
            return ctx_.context<T>();
        }

    };
}


#endif //REIMU_TCPCONN_H
