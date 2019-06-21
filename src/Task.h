//
// Created by 苏畅 on 2019/6/21.
//

#ifndef REIMU_TASK_H
#define REIMU_TASK_H

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
            _loop = loop;
        }

        void SetId(int id) { _id = id; };

        void SetStatus(TaskStatus status) { _status = status; };

        int GetId() {
            return _id;
        }

        int GetStatus() { return _status; }

    public:
        TaskCallBack _cb;

    protected:
        int _id;
        TaskStatus _status = TaskStatus::IDLE;

        EventLoop *_loop;
    };

    // 定时任务
    class Timer : public Task {
    private:
        time_t _run_at;
        time_t _repeat;
    public:
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

        int Cancel();


    };

}


#endif //REIMU_TASK_H
