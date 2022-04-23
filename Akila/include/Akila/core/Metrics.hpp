#pragma once

#include <map>
#include <string>
#include <chrono>

#ifdef WIN32

constexpr const char *file_name(const char *path) {
	const char *file = path;
	while(*path) {
		if(*path++ == '\\') {
			file = path;
		}
	}
	return file;
}

#else

constexpr const char *file_name(const char *path) {
	const char *file = path;
	while(*path) {
		if(*path++ == '/') {
			file = path;
		}
	}
	return file;
}

#endif

namespace Akila {
	class TimeMetric {
	private:
		friend class Timer;

		struct TimerData {
			TimerData();
			unsigned int count;
			std::chrono::microseconds msTotal;
		};

		static std::map<std::string, TimerData> timers;

	public:
		class Timer {
		private:
			std::string name;
			std::chrono::steady_clock::time_point begin;

		public:
			Timer(std::string const &name);
			~Timer();
		};

		static Timer scope(std::string const &name);
		static void flushAndDrawImGui();
	};
}

#define TIMER_NAME std::string(file_name(__FILE__)) +"::"+ std::string(__func__)
#define FUNC_TIME_METRIC() Akila::TimeMetric::Timer akilaTmpFuncTimer = Akila::TimeMetric::scope(TIMER_NAME);
