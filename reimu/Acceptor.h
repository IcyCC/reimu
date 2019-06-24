//
// Created by 苏畅 on 2019/6/24.
//

#ifndef REIMU_ACCEPTOR_H
#define REIMU_ACCEPTOR_H

#include "reimu_imp.h"

namespace reimu {
    class Acceptor : public noncopyable {
    public:
        std::unique_ptr<Channel> _channel;
        std::function<void (SocketPtr)> _accept_cb;
        EventLoop* _loop;
        IPv4Addr _listen_addr;
    
    public:
        Acceptor(EventLoop * loop, const IPv4Addr & lAddr) ;
        void OnAccept(const std::function<void (SocketPtr )> & cb ) {
            _accept_cb = cb;
        }

        void HandleRead();

        void Listen();

        void Close();

        void Cleanup();

        ~Acceptor();
    };
}



#endif //REIMU_ACCEPTOR_H
