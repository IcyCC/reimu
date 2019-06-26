//
// Created by 苏畅 on 2019/6/21.
//

#include "../reimu/reimu.h"
#include <iostream>

int main() {
    auto loop = reimu::EventLoop::GetInstance();



    auto t2 = loop->CallLater([]() {
        std::cout << "task 2" << std::endl;
    }, 1000);

    auto t3 = loop->CallRepeat([]() {
        std::cout << "task 3" << std::endl;
    }, 2000);

    auto t1 = loop->CallLater([t3]() {
        std::cout << "task 1" << std::endl;
        t3->Cancel();
    }, 8000);

    auto t4 = loop->CallLater([t3]() {
        std::cout << "task 4" << std::endl;
        t3->Cancel();
    }, 8000);

    t4->Cancel();

    loop->Loop();
}