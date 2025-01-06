#ifndef _WorkSteal_H_
#define _WorkSteal_H_

#include <atomic>
#include <cassert>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>
#include <map>
#include <cstdlib>
#include <sys/syscall.h>


typedef std::function<void()> ThreadTask;

namespace LFThreadPool{

class WorkItem{
public:
    WorkItem() {}
    WorkItem(bool s, const ThreadTask & f) : can_steal(s), fn(f) {}
private:
    bool can_steal;
    ThreadTask fn;
}

class WorkSteal{
public:
    explicit WorkSteal() {}
    explicit WorkSteal(size_t thread_num = std::thread::hardware_concurrency() + 1,
                       size_t work_size = -1,
                       bool enable_steal = true,
                       bool core_bind = false);
    ~WorkSteal();
    void Start() const {}
    void Stop();
    bool AddTask(ThreadTask &&fn, int32_t id = -1);
    bool AddTask(ThreadTask &&task, const std::string &name);
    
private:


};
};

#endif