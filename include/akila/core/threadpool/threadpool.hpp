#pragma once

#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>

namespace akila {
	class Threadpool {
	public:
		struct InitValues {
			unsigned int maxThreadCount = 4;
		};

		using Func = std::function<void()>;

		static void submit(Func const asyncPart, Func const syncPart = []() {});

	private:
		friend class Core;

		struct Task {
			Func main;
			Func join;
		};

		static std::mutex cm;
		static std::condition_variable cv;
		static std::vector<std::thread> threads;

		static std::mutex waitingTaskQueueMutex;
		static std::queue<Task> waitingTaskQueue;
		static std::mutex finishedTaskQueueMutex;
		static std::queue<Task> finishedTaskQueue;

		static bool exit;
		
		static void init(InitValues const &initVals);
		static void flush();
		static void terminate();
	};
}
