//
// Created by 苏畅 on 2019/6/21.
//

#ifndef REIMU_THREADINGPOOL_H
#define REIMU_THREADINGPOOL_H
#include "reimu_imp.h"
#include "SafeQueue.h"

namespace reimu {

    // 线程池， 通过SafeQueue 通信
    class ThreadingPool :public noncopyable{
    public:
        ThreadingPool(int  threads, int wait_ms=0);
        ~ThreadingPool();
    public:
        void Start();
        void Exit();
        bool AddTask(TaskPtr t);
    private:
        int _waitMs;
        SafeQueue<TaskPtr> _tasks;
        std::vector<std::thread> _threads;
    };
}

#endif //REIMU_THREADINGPOOL_H
