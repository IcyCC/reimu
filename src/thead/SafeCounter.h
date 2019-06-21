//
// Created by 苏畅 on 2019/6/21.
//

#ifndef REIMU_SAFECOUNTER_H
#define REIMU_SAFECOUNTER_H

#include "reimu_imp.h"

namespace reimu {
    // 线程安全计数器
    class SafeCounter {

    private:
        std::atomic<int> _value;
    public:
        SafeCounter() {
            _value = 1;
        }
    public:
        int Add(){
            return _value++;
        };

        int Read(){
            return _value;
        }
    };

    inline SafeCounter REIMU_GLOBAL_COUNTER = SafeCounter();
}



#endif //REIMU_SAFECOUNTER_H
