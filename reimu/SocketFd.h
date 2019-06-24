//
// Created by 苏畅 on 2019/6/19.
//

#ifndef REIMU_SOCKETFD_H
#define REIMU_SOCKETFD_H

#include "reimu_imp.h"

#include <netinet/in.h>
#include <netinet/tcp.h>

namespace reimu {

    struct IPv4Addr {
        struct sockaddr_in addr_;
        IPv4Addr(){};
        IPv4Addr(const std::string &host, unsigned short port);
        IPv4Addr(const struct sockaddr_in &addr) : addr_(addr) {};
    };

    inline const IPv4Addr DeafultLocaAddr = IPv4Addr();

    class Socket : public noncopyable{
    public:
        int _fd;
        IPv4Addr _addr;
    public:
       Socket(int socket_fd, const IPv4Addr & addr)
                : _fd(socket_fd){
            _addr = addr;
        }

        Socket() {
            _fd = socket(AF_INET, SOCK_STREAM , 0);
            if (_fd == -1) {
                perror(strerror(errno));
            }
            int flags = fcntl(_fd, F_GETFL, 0);
            fcntl(_fd, F_SETFL, flags | O_NONBLOCK | O_CLOEXEC);
            SetReuseAddr(true);
        }

        ~Socket() {
            close(_fd);
        }

        int fd() const { return _fd; }

    public:
        void BindAddress(const IPv4Addr& localaddr);
        void Listen();

        int Accept(IPv4Addr* peeraddr);

        void ShutdownWrite();

        void SetTcpNoDelay(bool on);

        void SetReuseAddr(bool on);

        void SetReusePort(bool on);

        void SetKeepAlive(bool on);

    };


}


#endif //REIMU_SOCKETFD_H
