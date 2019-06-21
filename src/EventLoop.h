//
// Created by 苏畅 on 2019/6/19.
//

#ifndef REIMU_EVENTLOOP_H
#define REIMU_EVENTLOOP_H

#include "reimu_imp.h"

namespace reimu {

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
        Task(EventLoop *loop, const TaskCallBack &cb) : _cb(cb) {
            _id = 0;
            _succ_cb = []() {};
            _loop = loop;
        }

        Task(EventLoop *loop, const TaskCallBack &cb, const TaskCallBack &succ_cb) : Task(loop, cb) {
            if (succ_cb) {
                _succ_cb = succ_cb;
            }
        }

        void SetId(int id) { id = _id; };

        void SetStatus(TaskStatus status) { _status = status; };

        int GetId() { return _id; }

        int GetStatus() { return _status; }

    public:
        TaskCallBack _cb;
        TaskCallBack _succ_cb; // 完成的回调

    protected:
        int _id;
        TaskStatus _status = TaskStatus::IDLE;

        EventLoop *_loop;
    };


    class EventLoopImpAbc : public noncopyable {
    public:
        EventLoop * _loop;
    public:
        // 任务相关接口 创建一个任务
        virtual TaskPtr CreateTask(const TaskCallBack &cb, const TaskCallBack &succ_cb = nullptr) = 0;

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
        TaskPtr CreateTask(const TaskCallBack &cb, const TaskCallBack &succ_cb);

    public:
        // 定时任务
        TimerPtr CallAt(const TaskCallBack &cb, time_t at);

        TimerPtr CallLater(const TaskCallBack &cb, time_t t);

        TimerPtr CallRepeat(const TaskCallBack &cb, time_t r);

        int CancelTimer(Timer *t);

        void Loop();


    private:
        std::unique_ptr<EventLoopImpAbc> _imp;
    };


    // 定时任务
    class Timer : public Task {
    private:
        time_t _run_at;
        time_t _repeat;
    public:
        Timer(EventLoop *loop, const TaskCallBack &cb, const TaskCallBack &succ_cb, time_t run_at, int repeat = -1)
                : Task(loop, cb, succ_cb) {
            _run_at = run_at;
            _repeat = repeat;
        };

        Timer(EventLoop *loop, const TaskCallBack &cb, time_t run_at, int repeat = -1) : Task(loop, cb) {
            _run_at = run_at;
            _repeat = repeat;
        };
    public:
        time_t GetRunAt() {
            return _run_at;
        }

        void SetRunAt(time_t at) {
            _run_at = at;
        }

        time_t GetRepeat() {
            return _repeat;
        }


    public:

        int Cancel() {
            return this->_loop->CancelTimer(this);
        }


    };


}


#endif //REIMU_EVENTLOOP_H
