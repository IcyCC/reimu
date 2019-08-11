//
// Created by 苏畅 on 2019/6/25.
//

//
// Created by 苏畅 on 2019/6/24.
//

#include "reimu.h"
#include <iostream>
#include <memory>

int main() {
    reimu::Signal::signal(SIGPIPE, [] {});

    auto loop = reimu::EventLoop::GetInstance();

    auto s = new reimu::TcpServer(loop, "0.0.0.0", 3099);

    s->OnConnMsg([](reimu::TcpConnPtr conn,  const std::string & msg) {
        conn->Send(msg);
    });

    s->OnConnDisconnected([](reimu::TcpConnPtr conn) {

    });
    s->OnAccept([](reimu::TcpConnPtr conn) {
    });

    s->StartServer();
    loop->Loop();
}