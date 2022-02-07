#include "Akila/core/ThreadPool.hpp"

using namespace Akila;

ThreadPool::ThreadPool(unsigned int count): exit{false} {
    unsigned int max = std::thread::hardware_concurrency();
    if(count == 0) count = max;
    else if(count > max) count = max;
    
    for(unsigned int i = 0; i < count; ++i) {
        threads.push_back(std::thread([this]() {

            while(!exit) {
                Job job;

                {
                    std::unique_lock<std::mutex> condLock(conditionMutex);
                    while(jobs.empty()) {
                        cv.wait(condLock);
                        if(exit) return;
                    }

                    {
                        std::lock_guard<std::mutex> lck(jobsMutex);
                        job = jobs.front();
                        jobs.pop();
                    }
                }

                job.task();

                {
                    std::lock_guard<std::mutex> lck(jobsMutex);
                    job.join();
                }
            }
        }));
    }
}

ThreadPool::~ThreadPool() {
    exit = true;
    cv.notify_all();
    for(int i = 0; i < threads.size(); i++) threads[i].join();
}

void ThreadPool::submit(std::function<void()> task, std::function<void()> join) {
    jobs.push({task, join});
    cv.notify_one();
}
