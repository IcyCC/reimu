//
// Created by 苏畅 on 2019/6/19.
//

#ifndef REIMU_EVENTLOOP_H
#define REIMU_EVENTLOOP_H

#include "reimu_imp.h"
#include "ThreadingPool.h"
#include "Task.h"

namespace reimu {




    class EventLoopImpAbc : public noncopyable {
    public:
        EventLoop * _loop;
    public:
        // 任务相关接口 创建一个任务
        virtual TaskPtr CreateTask(const TaskCallBack &cb) = 0;
        virtual void CallInThreading(TaskPtr task) = 0;

    public:
        // 定时任务相关接口
        virtual TimerPtr CallAt(const TaskCallBack &cb, time_t at) = 0;

        virtual TimerPtr CallLater(const TaskCallBack &cb, time_t t) = 0;

        virtual TimerPtr CallRepeat(const TaskCallBack &cb, time_t r) = 0;

        virtual int CancelTimer(Timer *t) = 0;

    public:
        virtual void loopTimer() = 0;

        virtual void loopIO() = 0;

        virtual void LoopOnce() {
            loopTimer();
            loopIO();
        }
    };


    class EventLoop : public noncopyable {
    public:
        EventLoop();

    public:

        // 任务相关
        TaskPtr CreateTask(const TaskCallBack &cb);

    public:
        // 定时任务
        TimerPtr CallAt(const TaskCallBack &cb, time_t at);

        TimerPtr CallLater(const TaskCallBack &cb, time_t t);

        TimerPtr CallRepeat(const TaskCallBack &cb, time_t r);

        int CancelTimer(Timer *t);

        void Loop();

        void CallInThreading(TaskPtr task);


    private:
        std::unique_ptr<EventLoopImpAbc> _imp;
    };






}


#endif //REIMU_EVENTLOOP_H
