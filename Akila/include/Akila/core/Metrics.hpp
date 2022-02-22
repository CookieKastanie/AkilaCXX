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
			std::chrono::milliseconds msTotal;
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

#define S1(x) #x
#define S2(x) S1(x)


//#define TIMER_NAME file_name(__FILE__) "::" S2(__FUNCTION_NAME__)
//#define TIMER_NAME file_name(__FILE__ "::" S2(__func__))
//#define TIMER_NAME file_name(__FILE__ "::" S2(__func__))
#define TIMER_NAME __func__
//#define FUNC_TIME_METRIC() Akila::TimeMetric::Timer tmpTimer = Akila::TimeMetric::scope(file_name(__FILE__##__FUNCTION_NAME__));
#define FUNC_TIME_METRIC() Akila::TimeMetric::Timer akilaTmpFuncTimer = Akila::TimeMetric::scope(TIMER_NAME);
