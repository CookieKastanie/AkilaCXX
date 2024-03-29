#include "akila/core/threadpool/threadpool.hpp"

using namespace akila;

std::mutex Threadpool::cm;
std::condition_variable Threadpool::cv;
std::vector<std::thread> Threadpool::threads;

std::mutex Threadpool::waitingTaskQueueMutex;
std::queue<Threadpool::Task> Threadpool::waitingTaskQueue;
std::mutex Threadpool::finishedTaskQueueMutex;
std::queue<Threadpool::Task> Threadpool::finishedTaskQueue;

bool Threadpool::exit;

void Threadpool::init(InitValues const &initVals) {
	exit = false;

	unsigned int threadCount = std::thread::hardware_concurrency();
	if(threadCount == 0) threadCount = 1;
	else if(threadCount > initVals.maxThreadCount) threadCount = initVals.maxThreadCount;

	for(unsigned int i = 0; i < threadCount; ++i) {
		threads.push_back(std::thread([]() {

			while(!exit) {
				{
					std::unique_lock<std::mutex> condLock(cm);
					while(waitingTaskQueue.empty()) {
						cv.wait(condLock);
						if(exit) return;
					}
				}

				Task task;

				{
					std::scoped_lock<std::mutex> lck(waitingTaskQueueMutex);
					if(waitingTaskQueue.empty()) continue;
					task = waitingTaskQueue.front();
					waitingTaskQueue.pop();
				}

				task.main();

				{
					std::scoped_lock<std::mutex> lck(finishedTaskQueueMutex);
					finishedTaskQueue.push(task);
				}
			}
		}));
	}
}

void Threadpool::submit(Func const asyncPart, Func const syncPart) {
	{
		std::scoped_lock<std::mutex> lck(waitingTaskQueueMutex);
		waitingTaskQueue.push({asyncPart, syncPart});
	}
	cv.notify_one();
}

void Threadpool::flush() {
	std::scoped_lock<std::mutex> lck(finishedTaskQueueMutex);

	while(!finishedTaskQueue.empty()) {
		Task task = finishedTaskQueue.front();
		task.join();
		finishedTaskQueue.pop();
	}
}

void Threadpool::terminate() {
	exit = true;
	cv.notify_all();
	for(int i = 0; i < threads.size(); i++) threads[i].join();

	threads.clear();
	std::queue<Threadpool::Task>().swap(waitingTaskQueue); // = clear
	std::queue<Threadpool::Task>().swap(finishedTaskQueue); // = clear
}
