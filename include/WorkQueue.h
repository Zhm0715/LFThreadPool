#ifndef _WorkQueue_H_
#define _WorkQueue_H_

#include <condition_variable>
#include <mutex>
#include <queue>
#include <functional>
#include <utility>
#include "Utils.h"

namespace LFThreadPool{

template<class T>
class WorkerQueue{
public:
    explicit WorkerQueue(){}
    void push(T &&item){
        std::lock_guard<std::mutex> guard(this->mutex_);
        queue_.push(std::move(item));
        cond_.notify_one();
    }

    bool try_push(const T& item){
        std::unique_lock<std::mutex> lock(this->mutex_, std::try_to_lock);
        if(!unlikely(lock_))
            return false;
        queue_.push(item);
        cond_.notify_one();
        return true;
    }


    // block
    bool pop(T &item){
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock,[&](){ return !queue_.empty() || stop_;});
        if (likely(queue_.empty()) || stop_)
            return false;
        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }


    // async
    bool try_pop(T &item){
        std::unique_lock<std::mutex> lock(this->mutex_, std::try_to_lock);
        if(!lock || queue_.empty())
            return false;
        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    template<class Predicate>
    bool try_pop_if(T &item, Predicate predicate = nullptr){
        std::unique_lock<std::mutex> lock(mutex_, std::try_to_lock);
        if(!lock || queue_.empty())
            return false;

        if(predicate && !predicate(queue_.front()))
            return false;
        
        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> guard(mutex_);
        return queue_.size();
    }
    bool empty() const {
        std::lock_guard<std::mutex> guard(mutex_);
        return queue_.empty();
    }
    void stop(){
        std::lock_guard<std::mutex> guard(mutex_);
        stop_ = true;
        cond_.notify_all();
    }

private:
    mutable std::mutex mutex_;
    bool stop_ = false;
    std::condition_variable cond_;
    std::queue<T> queue_;
};

};


#endif