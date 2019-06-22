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
}