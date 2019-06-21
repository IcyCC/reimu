//
// Created by 苏畅 on 2019/6/19.
//

#include "EventLoop.h"


namespace reimu {


    class DefaultEventLoopImp : public EventLoopImpAbc {
    public:
        DefaultEventLoopImp(EventLoop *loop) {
            _loop = loop;
        }

    public:
        // 任务相关
        TaskPtr CreateTask(const TaskCallBack &cb, const TaskCallBack &succ_cb = nullptr) override;

    public:
        TimerPtr CallAt(const TaskCallBack &cb, time_t at) override;

        TimerPtr CallLater(const TaskCallBack &cb, time_t t) override;

        TimerPtr CallRepeat(const TaskCallBack &cb, time_t r) override;

        void loopTimer() override;

        void loopIO() override;

        int CancelTimer(Timer *t) final;


    private:
        std::map<int, TaskPtr> _tasks;

        typedef std::pair<time_t, int> TimerKey;

        class TimerKeyCompare {
        public:
            bool operator()(const TimerKey &t1, const TimerKey &t2) const {
                return t1.first > t2.first;
            }
        };

        std::map<TimerKey, TimerPtr, TimerKeyCompare> _timers;
    };


    TaskPtr
    DefaultEventLoopImp::CreateTask(const reimu::TaskCallBack &cb, const reimu::TaskCallBack &succ_cb) {
        auto t = std::make_shared<Task>(this->_loop, cb, succ_cb);
        int task_id = 1;
        t->SetStatus(Task::TaskStatus::PENDING);
        t->SetId(task_id);
        _tasks.insert(std::make_pair(task_id, t));
        return t;
    }

    TimerPtr DefaultEventLoopImp::CallAt(const reimu::TaskCallBack &cb, time_t at) {
        auto t = std::make_shared<Timer>(this->_loop, cb, at, -1);
        int timer_id = 1;
        t->SetId(timer_id);
        _timers.insert(std::make_pair(std::make_pair(at, timer_id), t));
        return t;
    }

    TimerPtr DefaultEventLoopImp::CallLater(const reimu::TaskCallBack &cb, time_t t) {
        time_t at = util::TimeMilli() + t;
        return CallAt(cb, at);
    }

    TimerPtr DefaultEventLoopImp::CallRepeat(const reimu::TaskCallBack &cb, time_t r) {
        time_t at = util::TimeMilli() + r;
        auto t = std::make_shared<Timer>(this->_loop, cb, at, r);
        int timer_id = 1;
        t->SetId(timer_id);
        _timers.insert(std::make_pair(std::make_pair(at, timer_id), t));
        return t;
    }

    int DefaultEventLoopImp::CancelTimer(reimu::Timer *t) {
        if (t->GetStatus() == Task::TaskStatus::PENDING) {
            t->SetStatus(Task::TaskStatus::CANCEL);
            _timers.erase(std::make_pair(t->GetRunAt(), t->GetId()));
            return 1;
        }
        return 0;
    }

    void DefaultEventLoopImp::loopTimer() {
        time_t current_time = util::TimeMilli();
        auto it = _timers.upper_bound(std::make_pair(current_time, 0));
        while (it != _timers.end()) {
            it->second->_cb();
            if (it->second->GetRepeat() > 0) {
                // 重复任务
                auto t = std::make_shared<Timer>(_loop, it->second->_cb, current_time + it->second->GetRepeat(),
                                                 it->second->GetRepeat());
                t->SetId(it->second->GetId());
                _timers.insert(
                        std::make_pair(std::make_pair(t->GetRunAt(), it->second->GetId()), t)
                );
            }
            _timers.erase(it++);
        }
    }

    void DefaultEventLoopImp::loopIO() {
        return;
    }

    EventLoop::EventLoop() {
        _imp = std::make_unique<DefaultEventLoopImp>(this);
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

    int EventLoop::CancelTimer(reimu::Timer *t) {
        return _imp->CancelTimer(t);
    }

    void EventLoop::Loop() {
        while (true) {
            _imp->LoopOnce();
        }
    }


}