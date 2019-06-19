//
// Created by 苏畅 on 2019/6/19.
//

#ifndef REIMU_SOCKETFD_H
#define REIMU_SOCKETFD_H

#include "reimu_imp.h"

class Socket {
public:
    int _fd;
public:
    SocketFd(int socket_fd)
    :_fd(socket_fd)
    {}

    +Socket() {
        close(_fd);
    }

    int fd() const { return _fd; }

};


#endif //REIMU_SOCKETFD_H
