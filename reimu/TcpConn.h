//
// Created by 苏畅 on 2019/6/22.
//

#ifndef REIMU_TCPCONN_H
#define REIMU_TCPCONN_H

#include "reimu_imp.h"

namespace reimu {
    class TcpConn : public std::enable_shared_from_this<TcpConn>, private noncopyable {
    private:
        std::unique_ptr<Channel> _channel;
    public:


    };
}


#endif //REIMU_TCPCONN_H
