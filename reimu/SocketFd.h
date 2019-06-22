//
// Created by 苏畅 on 2019/6/19.
//

#ifndef REIMU_SOCKETFD_H
#define REIMU_SOCKETFD_H

#include "reimu_imp.h"

namespace reimu {

    struct IPv4Addr {
        struct sockaddr_in addr_;

        IPv4Addr(const std::string &host, unsigned short port);

        IPv4Addr(const struct sockaddr_in &addr) : addr_(addr) {};
    };

    class Socket : public noncopyable{
    public:
        int _fd;
    public:
        explicit Socket(int socket_fd)
                : _fd(socket_fd) {}

        Socket() {
            _fd = socket(AF_INET, SOCK_STREAM | SO_KEEPALIVE | SO_REUSEPORT, 0);
            int flags = fcntl(_fd, F_GETFL, 0);
            fcntl(_fd, F_SETFL, flags | O_NONBLOCK | O_CLOEXEC);
        }

        ~Socket() {
            close(_fd);
        }

        int fd() const { return _fd; }

    };


}


#endif //REIMU_SOCKETFD_H
