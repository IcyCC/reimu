//
// Created by 苏畅 on 2019/6/21.
//

#ifndef REIMU_SAFEQUEUE_H
#define REIMU_SAFEQUEUE_H

#include "../reimu_imp.h"

namespace reimu {
    template <typename  T>
    // 线程安全队列
    class SafeQueue: private  std::mutex, private noncopyable {
    public:
        static const int wait_infinite = std::numeric_limits<int>::max(); //  最大等待时间

        SafeQueue() : exit_(false) {}

        bool Push(T && v);

        bool Push(const T &v);

        T PopWait(int waitMs = wait_infinite);

        size_t  Size();

        void Exit();
        bool Exited(){ return exit_; };

    private:
        std::list<T> items_;
        std::condition_variable ready_;
        std::atomic<bool> exit_;
        void waitReady(std::unique_lock<std::mutex> &lk, int waitMs);
    };


    template <typename T>
    size_t SafeQueue<T>::Size() {
        std::lock_guard<std::mutex> lk(*this);
        return items_.size();
    }


    template <typename T>
    void SafeQueue<T>::Exit() {
        exit_ = true;
        std::lock_guard<std::mutex> lk(*this);
        ready_.notify_all();
    }

    template <typename T>
    bool SafeQueue<T>::Push(T &&v) {
        Push(v);
        return true;
    }

    template <typename T>
    bool SafeQueue<T>::Push(const T &v) {
        std::lock_guard<std::mutex> lk(*this);
        if(exit_ ) {
            return false;
        }
        items_.push_back(v);
        ready_.notify_one();
        return true;
    }

    template <typename T>
    void SafeQueue<T>::waitReady(std::unique_lock<std::mutex> &lk, int waitMs) {
        if (exit_ || !items_.empty() ) {
            return ;
        }

        if (waitMs == wait_infinite) {
            ready_.wait(lk, [this] { return exit_ || !items_.empty(); });
        } else {
            auto tp = std::chrono::steady_clock::now() + std::chrono::milliseconds(waitMs);
            while (ready_.wait_until(lk, tp) != std::cv_status::timeout && items_.empty() && !exit_) {
            }
        }
    }

    template <typename T>
    T SafeQueue<T>::PopWait(int waitMs) {
        std::unique_lock<std::mutex> lk(*this);
        waitReady(lk, waitMs);
        if (items_.empty()){
            return T();
        }
        T r =  std::move(items_.front());
        items_.pop_front();
        return r;
    }
}

#endif //REIMU_SAFEQUEUE_H
