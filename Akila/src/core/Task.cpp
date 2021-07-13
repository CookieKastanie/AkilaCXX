#include "Akila/core/Task.hpp"

using namespace Akila;

TaskManager::TaskManager() {
    threadPool.start(this);
}

TaskManager::~TaskManager() {

}

void TaskManager::submit(std::shared_ptr<Task> task) {
    threadPool.submit(task);
}

void TaskManager::submit(Task *task) {
	submit(std::shared_ptr<Task>(task));
}

void TaskManager::submitSync(std::shared_ptr<Task> task) {
    std::lock_guard<std::mutex> lck(queueMutex);
    tasks.push(task);
}

void TaskManager::submitSync(Task *task) {
    submitSync(std::shared_ptr<Task>(task));
}

void TaskManager::flush() {
    std::lock_guard<std::mutex> lck(queueMutex);
    while(!tasks.empty()) {
        auto t = tasks.front();
        tasks.pop();
        t->onMain();
    }
}


///////////////////////////////////////////////////////////////////////////////////////////


ThreadPool::ThreadPool(): exit{false} {
    
}

void ThreadPool::start(TaskManager *taskManager, unsigned int max) {
    unsigned int count = std::thread::hardware_concurrency();
    if(count > max) count = max;
    if(count == 0) count = 1;

    for(unsigned int i = 0; i < count; ++i) {
        threads.push_back(std::thread([this, taskManager]() {
            
            while(!exit) {
                std::shared_ptr<Task> t;

                {
                    std::unique_lock<std::mutex> condLock(conditionMutex);
                    while(tasks.empty()) {
                        cv.wait(condLock);
                        if(exit) return;
                    }

                    std::lock_guard<std::mutex> lck(queueMutex);
                    t = tasks.front();
                    tasks.pop();
                }

                t->onBackground();

                {
                    std::lock_guard<std::mutex> lck(taskManager->queueMutex);
                    taskManager->tasks.push(t);
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

void ThreadPool::submit(std::shared_ptr<Task> task) {
    std::lock_guard<std::mutex> lck(queueMutex);
    tasks.push(task);
    cv.notify_all();
}


///////////////////////////////////////////////////////////////////////////////////////////


Task::Task() {}

Task::~Task() {}

void Task::onBackground() {}

void Task::onMain() {}
