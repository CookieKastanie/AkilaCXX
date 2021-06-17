#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

namespace Akila {
	class Task {
		public:
			Task();
			virtual ~Task();

			virtual void onBackGround();
			virtual void onMain();
	};

	class TaskManager;
	class ThreadPool {
		private:
			friend class TaskManager;

			bool exit;
			std::vector<std::thread> threads;

			std::mutex conditionMutex;
			std::condition_variable cv;

			std::mutex queueMutex;
			std::queue<std::shared_ptr<Task>> tasks;

			void start(TaskManager *taskManager, unsigned int max = 4);
			void submit(std::shared_ptr<Task> task);

		public:
			ThreadPool();
			~ThreadPool();
	};

	class TaskManager {
		private:
			friend class ThreadPool;

			ThreadPool threadPool;

			std::mutex queueMutex;
			std::queue<std::shared_ptr<Task>> tasks;

		public:
			TaskManager();
			~TaskManager();

			void submit(std::shared_ptr<Task> task);
			void submit(Task *task);

			void flush();
	};
}
