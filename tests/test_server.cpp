//
// Created by 苏畅 on 2019/6/25.
//

//
// Created by 苏畅 on 2019/6/24.
//

#include "../reimu/reimu.h"
#include <iostream>
#include <memory>

int main() {
    auto loop = reimu::EventLoop::GetInstance();

    auto s = new reimu::TcpServer(loop, "0.0.0.0", 10011);

    s->OnConnMsg([](reimu::TcpConnPtr conn, reimu::Slice s) {

        std::cout << "收到消息" << s.toString() << std::endl;
        conn->Send("hello");
    });

    s->OnConnDisconnected([](reimu::TcpConnPtr conn) {
        std::cout << "断开链接"<<std::endl;

    });
    s->OnAccept([](reimu::TcpConnPtr conn) {
        std::cout << "收到链接"<< "来自 "<<conn->GetDestAddr().ToString()<<std::endl;
    });

    s->StartServer();
    loop->Loop();
}