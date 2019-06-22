//
// Created by 苏畅 on 2019/6/21.
//

#ifndef REIMU_SAFECOUNTER_H
#define REIMU_SAFECOUNTER_H

#include "../reimu_imp.h"

namespace reimu {
    // 线程安全计数器
    class SafeCounter {

    private:
        std::atomic<long> _value;
    public:
        SafeCounter() {
            _value = 1;
        }
    public:
        long Add(){
            return _value++;
        };

        long Read(){
            return _value;
        }
    };

    inline SafeCounter REIMU_GLOBAL_COUNTER = SafeCounter();
}



#endif //REIMU_SAFECOUNTER_H
