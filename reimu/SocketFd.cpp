//
// Created by 苏畅 on 2019/6/19.
//

#include "SocketFd.h"


namespace reimu {

    IPv4Addr::IPv4Addr(const std::string &host, unsigned short port) {
        memset(&addr_, 0, sizeof addr_);
        addr_.sin_family = AF_INET;
        addr_.sin_port = htons(port);
        if (host.empty()) {
            addr_.sin_addr.s_addr = INADDR_ANY;
        } else {
            if (!inet_aton(host.c_str(), &(addr_.sin_addr))) {
                exit(1);
            }
        }
    }


    void Socket::SetTcpNoDelay(bool on)
    {
        int optval = on ? 1 : 0;
        ::setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY,
                     &optval, static_cast<socklen_t>(sizeof optval));
        // FIXME CHECK
    }

    void Socket::SetReuseAddr(bool on)
    {
        int optval = on ? 1 : 0;
        ::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR,
                     &optval, static_cast<socklen_t>(sizeof optval));
        // FIXME CHECK
    }

    void Socket::SetReusePort(bool on)
    {
#ifdef SO_REUSEPORT
        int optval = on ? 1 : 0;
        int ret = ::setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT,
                               &optval, static_cast<socklen_t>(sizeof optval));
#else
        if (on)
  {
    LOG_ERROR << "SO_REUSEPORT is not supported.";
  }
#endif
    }

    void Socket::SetKeepAlive(bool on) {
        int optval = on ? 1 : 0;
        ::setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE,
                     &optval, static_cast<socklen_t>(sizeof optval));
    }

    void Socket::BindAddress(const IPv4Addr &localaddr) {
        _addr = localaddr;
        int ret = ::bind(_fd, (struct sockaddr *) &_addr.addr_, sizeof(struct sockaddr));
    }

    int Socket::Accept(reimu::IPv4Addr *peeraddr) {
        struct sockaddr_in raddr;
        socklen_t rsz = sizeof(raddr);
        int ret = ::accept(_fd, (struct sockaddr *) &raddr, &rsz);
        peeraddr->addr_ = raddr;
        return ret;
    }

    void Socket::Listen() {
        ::listen(_fd, SOMAXCONN);
    }

    void Socket::ShutdownWrite() {
        ::shutdown(_fd, SHUT_WR);
    }


}