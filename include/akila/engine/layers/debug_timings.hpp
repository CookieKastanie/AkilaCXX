#pragma once

#include <map>
#include <string>
#include <chrono>

namespace akila::internal {
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

	class TimeMetric {
	public:
		class Timer {
		public:
			Timer(std::string const &name);
			~Timer();
		private:
			std::string name;
			std::chrono::steady_clock::time_point begin;
		};

		static Timer scope(std::string const &name);
		static void flushAndDrawImGui();

	private:
		friend class Timer;

		struct TimerData {
			TimerData();
			unsigned int count;
			std::chrono::microseconds msTotal;
		};

		static std::map<std::string, TimerData> timers;
	};
}

#define TIMER_NAME std::string(akila::internal::file_name(__FILE__)) +"::"+ std::string(__func__)
#define FUNC_TIME_METRIC() akila::internal::TimeMetric::Timer akilaTmpFuncTimer = akila::internal::TimeMetric::scope(TIMER_NAME);
