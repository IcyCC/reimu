//
// Created by 苏畅 on 2019/6/19.
//

#include "EventLoop.h"
#include "../Channel.h"


namespace reimu {


    class DefaultEventLoopImp : public EventLoopImpAbc {
    public:
        DefaultEventLoopImp(EventLoop *loop) {
            _loop = loop;
            _threading_pool = std::make_shared<ThreadingPool>(5, -1);
            _threading_pool->Start();
        }

    public:
        // 任务相关
        TaskPtr CreateTask(const TaskCallBack &cb) override;

        void CallInThreading(TaskPtr task) override;

    public:
        TimerPtr CallAt(const TaskCallBack &cb, time_t at) override;

        TimerPtr CallLater(const TaskCallBack &cb, time_t t) override;

        TimerPtr CallRepeat(const TaskCallBack &cb, time_t r) override;

        void loopTimer() override;

        void loopIO() override;

        int CancelTimer(Timer *t) final;

        Poller *GetPoller() override;

        void SetPoller(Poller *p) override;


    private:
        typedef std::pair<time_t, int> TimerKey;

        class TimerKeyCompare {
        public:
            bool operator()(const TimerKey &t1, const TimerKey &t2) const {
                return t1.first > t2.first;
            }
        };

        std::map<TimerKey, TimerPtr, TimerKeyCompare> _timers;

        std::shared_ptr<ThreadingPool> _threading_pool;
    };


    TaskPtr
    DefaultEventLoopImp::CreateTask(const reimu::TaskCallBack &cb) {
        auto t = std::make_shared<Task>(this->_loop, cb);
        int task_id = REIMU_GLOBAL_COUNTER.Add();
        t->SetStatus(Task::TaskStatus::PENDING);
        t->SetId(task_id);
        CallInThreading(t);
        return t;
    }

    TimerPtr DefaultEventLoopImp::CallAt(const reimu::TaskCallBack &cb, time_t at) {
        auto t = std::make_shared<Timer>(this->_loop, cb, at, -1);
        int timer_id = REIMU_GLOBAL_COUNTER.Add();
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
        int timer_id = REIMU_GLOBAL_COUNTER.Add();
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

    void DefaultEventLoopImp::CallInThreading(reimu::TaskPtr task) {
        _threading_pool->AddTask(task);
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
        if (_poller == nullptr) {
            return;
        } else {
            ChannelList activeChannels;
            _poller->Poll(-1, activeChannels);
            for (auto &c : activeChannels) {
                // 并发处理
                CreateTask([&](){
                    if (c->GetEvents() & _poller->REIMU_POLLIN) {
                        c->HandleRead();
                    }
                    if (c->GetEvents() & _poller->REIMU_POLLOUT) {
                        c->HandleWrite();
                    }
                    if (c->GetEvents() & _poller->REIMU_POLLERR) {
                        c->HandleError();
                    }
                });
            }
        }
    }

    Poller *DefaultEventLoopImp::GetPoller() {
        return _poller.get();
    }

    void DefaultEventLoopImp::SetPoller(reimu::Poller *p) {
        _poller.reset(p);
    }


    EventLoop::EventLoop() {
        _imp = std::make_unique<DefaultEventLoopImp>(this);
    }

    TaskPtr EventLoop::CreateTask(const reimu::TaskCallBack &cb) {
        return _imp->CreateTask(cb);
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

    void EventLoop::CallInThreading(reimu::TaskPtr task) {
        return _imp->CallInThreading(task);
    }

    void EventLoop::SetPoller(reimu::Poller *p) {
        return _imp->SetPoller(p);
    }

    void EventLoop::AddChannel(Channel* ch) {
        return _imp->AddChannel(ch);
    };

    void EventLoop::RemoveChannel(Channel* ch) {
        return _imp->RemoveChannel(ch);
    };

    void EventLoop::UpdateChannel(Channel* ch){
        return _imp->UpdateChannel(ch);
    };


    Poller *EventLoop::GetPoller() {
        return _imp->GetPoller();
    }

    int Timer::Cancel() {
        return this->_loop->CancelTimer(this);
    }


}