//
// Created by 苏畅 on 2019/6/24.
//

#include "reimu/reimu.h"
#include <iostream>
#include <memory>

int main() {
    auto loop = reimu::EventLoop::GetInstance();

    auto c = std::make_shared<reimu::TcpConn>(&loop, 10);

    c->OnConnected([](reimu::TcpConnPtr conn ){
        std::cout<<"链接成功"<<std::endl;
        conn->Send("hello");
    });

    c->OnMsg([](reimu::TcpConnPtr conn, reimu::Slice s ){

        std::cout<<"收到消息"<<s.toString()<<std::endl;
    });

    c->Connect("0.0.0.0", 10010);


    loop->Loop();
}