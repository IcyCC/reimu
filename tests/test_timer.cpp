//
// Created by 苏畅 on 2019/6/21.
//

#include "../reimu/reimu.h"
#include <iostream>

int main() {
    auto loop = reimu::EventLoop();

    auto t1 = loop.CallLater([]() {
        std::cout << "task 1" << std::endl;
    }, 5000);

    auto t2 = loop.CallLater([]() {
        std::cout << "task 2" << std::endl;
    }, 1000);

    auto t3 = loop.CallRepeat([]() {
        std::cout << "task 3" << std::endl;
    }, 2000);

    loop.Loop();
}