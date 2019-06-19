//
// Created by 苏畅 on 2019/6/19.
//

#ifndef REIMU_EVENTLOOP_H
#define REIMU_EVENTLOOP_H

#include "reimu_imp.h"

namespace reimu {
    typedef std::function<void()> TaskCallBack;
    // 任务对象 表达一个异步的任务， 可以添加回调
    class Task : public noncopyable {
    public:
        enum TaskStatus {
            IDLE,
            PENDING,
            CANCEL,
            FINISH
        };

    public:
        Task(const TaskCallBack &cb) : _cb(cb) {
            _id = 0;
            _succ_cb = [](){};
        }

        Task(const TaskCallBack &cb, const TaskCallBack &succ_cb) : Task(cb) {
            if (succ_cb) {
                _succ_cb = succ_cb;
            }
        }

        void SetId(int id) { id = _id; };

        void SetStatus(TaskStatus status) { _status = status; };

        int GetId() { return _id; }

        int GetStatus() { return _status; }

    public:

    protected:
        int _id;
        TaskStatus _status = TaskStatus::IDLE;
        TaskCallBack _cb;
        TaskCallBack _succ_cb; // 完成的回调
    };

    // 定时任务
    class Timer : public Task {
    private:
        time_t _run_at;
        time_t _repeat;
    public:
        Timer(const TaskCallBack &cb, const TaskCallBack &succ_cb, time_t run_at, int repeat = -1)  : Task(cb, succ_cb) {
            _run_at = run_at;
            _repeat = repeat;
        };

        Timer(const TaskCallBack &cb, time_t run_at, int repeat = -1) : Task(cb) {
            _run_at = run_at;
            _repeat = repeat;
        };
    };

    class EventLoopImpAbc : public noncopyable {
    public:
        // 任务相关接口 创建一个任务
        virtual TaskPtr CreateTask(const TaskCallBack &cb, const TaskCallBack &succ_cb = nullptr) = 0;
    public:
        // 定时任务相关接口
        virtual TimerPtr CallAt(const TaskCallBack &cb, time_t at) = 0;
        virtual TimerPtr CallLater(const TaskCallBack &cb, time_t t) = 0;
        virtual TimerPtr CallRepeat(const TaskCallBack &cb, time_t r) = 0;
    };


    class EventLoop : public noncopyable {
    public:
        EventLoop();

    public:

        // 任务相关
        TaskPtr CreateTask(const TaskCallBack &cb, const TaskCallBack &succ_cb);

    public:
        // 定时任务
        TimerPtr CallAt(const TaskCallBack &cb, time_t at);
        TimerPtr CallLater(const TaskCallBack &cb, time_t t);
        TimerPtr CallRepeat(const TaskCallBack &cb, time_t r);

    private:
        std::unique_ptr<EventLoopImpAbc> _imp;
    };

}


#endif //REIMU_EVENTLOOP_H
