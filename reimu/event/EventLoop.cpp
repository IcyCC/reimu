//
// Created by 苏畅 on 2019/6/19.
//

#include "EventLoop.h"
#include "poller/Poller.h"
#include "thead/ThreadingPool.h"
#include "Task.h"
namespace reimu {


    class DefaultEventLoopImp : public EventLoopImpAbc {
    public:
        DefaultEventLoopImp(EventLoop *loop) {
            _loop = loop;
            _threading_pool = std::make_shared<ThreadingPool>(4,-1);
            _threading_pool->Start();
            _poller = std::make_unique<Poller>();
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

        int _poll_wait = 100;
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
        t->SetStatus(Task::TaskStatus::PENDING);

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
        t->SetStatus(Task::TaskStatus::PENDING);
        t->SetId(timer_id);
        _timers.insert(std::make_pair(std::make_pair(at, timer_id), t));
        return t;
    }

    int DefaultEventLoopImp::CancelTimer(reimu::Timer *t) {
        if (t->GetStatus() == Task::TaskStatus::PENDING) {
            t->SetStatus(Task::TaskStatus::CANCEL);
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
            if (it->second->GetStatus() == Task::TaskStatus::PENDING){
                CreateTask(it->second->_cb);
                if (it->second->GetRepeat() > 0) {
                    // 重复任务
                    it->second->SetRunAt(it->second->GetRunAt() + it->second->GetRepeat());
                    _timers.insert(
                            std::make_pair(std::make_pair(it->second->GetRunAt(), it->second->GetId()), it->second)
                    );
                }
            }
            _timers.erase(it++);
        }
    }

    void DefaultEventLoopImp::loopIO() {
        if (_poller == nullptr) {
            return;
        } else {
            const int  wait_offset = 100;
            ChannelList activeChannels;
//            time_t current_time = util::TimeMilli();
//            int timer_wait = _poll_wait;
//            auto it = _timers.upper_bound(std::make_pair(current_time + wait_offset ,  0));
//            if (it != _timers.end()) {
//                timer_wait = it->first.first  - current_time;
//            }

            _poller->Poll(100, activeChannels);
            for (auto &c : activeChannels) {
                // 并发处理
                if (c->GetEvents() & (_poller->REIMU_POLLIN | _poller->REIMU_POLLERR)) {
                    c->HandleRead();
                }
                if (c->GetEvents() & (_poller->REIMU_POLLOUT)) {
                    c->HandleWrite();
                } else {
                    // 异常的事件
                }
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

    void EventLoop::AddChannel(Channel *ch) {
        return _imp->AddChannel(ch);
    };

    void EventLoop::RemoveChannel(Channel *ch) {
        return _imp->RemoveChannel(ch);
    };

    void EventLoop::UpdateChannel(Channel *ch) {
        return _imp->UpdateChannel(ch);
    };


    EventLoop* EventLoop::GetInstance() {
        std::mutex l;
        std::lock_guard<std::mutex> lock(l);
        if (_loop == nullptr) {
            _loop = new EventLoop();
        }
        return _loop;
    }


    Poller *EventLoop::GetPoller() {
        return _imp->GetPoller();
    }

    int Timer::Cancel() {
        if (this != nullptr){
            return this->_loop->CancelTimer(this);
        }
    }


}