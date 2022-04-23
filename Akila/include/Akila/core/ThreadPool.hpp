#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>

namespace Akila {
	class ThreadPool {
	private:
		std::vector<std::thread> threads;

		std::mutex conditionMutex;
		std::condition_variable cv;

		struct Job {
			std::function<void()> task;
			std::function<void()> join;
		};

		std::queue<Job> jobs;
		std::mutex jobsMutex;

		bool exit;

	public:
		ThreadPool(unsigned int count = 0);
		~ThreadPool();
		void submit(std::function<void()> task, std::function<void()> join = []() {});
	};
}
