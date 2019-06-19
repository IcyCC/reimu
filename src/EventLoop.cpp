//
// Created by 苏畅 on 2019/6/19.
//

#include "EventLoop.h"


namespace reimu {


    class DefaultEventLoopImp : public EventLoopImpAbc {
    public:
        // 任务相关
        TaskPtr CreateTask(const TaskCallBack &cb, const TaskCallBack &succ_cb = nullptr) final;

    public:
        TimerPtr CallAt(const TaskCallBack &cb, time_t at) final;
        TimerPtr CallLater(const TaskCallBack &cb, time_t t) final;
        TimerPtr CallRepeat(const TaskCallBack &cb, time_t r) final;

    private:
        std::map<int, TaskPtr> _tasks;

        typedef std::pair<time_t, int> TImerKey;
        std::map<TImerKey, TimerPtr> _timers;
    };


    TaskPtr
    DefaultEventLoopImp::CreateTask(const reimu::TaskCallBack &cb, const reimu::TaskCallBack &succ_cb) {
        auto t = std::make_shared<Task>(cb, succ_cb);
        int task_id = 1;
        t->SetStatus(Task::TaskStatus::PENDING);
        t->SetId(task_id);
        _tasks.insert(std::make_pair(task_id, t));
        return t;
    }

    TimerPtr DefaultEventLoopImp::CallAt(const reimu::TaskCallBack &cb, time_t at) {
        auto t = std::make_shared<Timer>(cb, at, -1);
        int timer_id = 1;
        t->SetId(timer_id);
        _timers.insert(std::make_pair(std::make_pair(at, timer_id), t));
        return  t;
    }

    TimerPtr DefaultEventLoopImp::CallLater(const reimu::TaskCallBack &cb, time_t t) {
        time_t  at = util::TimeMicro() + t;
        return CallAt(cb, at);
    }

    TimerPtr DefaultEventLoopImp::CallRepeat(const reimu::TaskCallBack &cb, time_t r) {
        time_t  at = util::TimeMicro() + r;
        auto t = std::make_shared<Timer>(cb, at, r);
        int timer_id = 1;
        t->SetId(timer_id);
        _timers.insert(std::make_pair(std::make_pair(at, timer_id), t));
        return  t;
    }

    EventLoop::EventLoop() {
        _imp = std::make_unique<DefaultEventLoopImp>();
    }

    TaskPtr EventLoop::CreateTask(const reimu::TaskCallBack &cb, const reimu::TaskCallBack &succ_cb) {
        return _imp->CreateTask(cb, succ_cb);
    }

    TimerPtr EventLoop::CallLater(const reimu::TaskCallBack &cb, time_t t) {
        return _imp->CallLater(cb, t);
    }

    TimerPtr EventLoop::CallAt(const reimu::TaskCallBack &cb, time_t at) {
        return _imp->CallAt(cb, at);
    }

    TimerPtr EventLoop::CallRepeat(const reimu::TaskCallBack &cb, time_t r) {
        return _imp->CallRepeat(cb, r);
    }
}