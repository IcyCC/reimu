//
// Created by 苏畅 on 2019/6/21.
//

#include "../reimu/reimu.h"
#include <iostream>

int main() {
    auto loop = reimu::EventLoop::GetInstance();

    auto t1 = loop->CreateTask([]() {
        std::cout << "task 1" << std::endl;
    });
    std::cout << "t1 "<<t1->GetId() << std::endl;

    auto t2 = loop->CreateTask([]() {
        std::cout << "task 2" << std::endl;
    });

    std::cout << "t2 "<<t2->GetId() << std::endl;

    loop->Loop();
}