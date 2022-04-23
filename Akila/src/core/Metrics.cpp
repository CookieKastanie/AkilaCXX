#include "Akila/core/Metrics.hpp"
#include "imgui/imgui.h"

using namespace Akila;

std::map<std::string, TimeMetric::TimerData> TimeMetric::timers{};

TimeMetric::TimerData::TimerData(): count{0}, msTotal{0} {

}

TimeMetric::Timer TimeMetric::scope(std::string const &name) {
	++timers[name].count;
	return {name};
}

TimeMetric::Timer::Timer(std::string const &name): name{name} {
	begin = std::chrono::steady_clock::now();
}

TimeMetric::Timer::~Timer() {
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	TimeMetric::timers[name].msTotal += std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
}

void TimeMetric::flushAndDrawImGui() {
	ImGui::Begin("Timers Stats");

	for(auto &t: timers) {
		std::string text{t.first + " : " +
			std::to_string(t.second.msTotal.count() / 1000.) + "ms (" +
			std::to_string(t.second.count) + ")"};
		ImGui::Text(text.c_str());

		t.second = TimeMetric::TimerData{};
	}
	
	ImGui::End();
}
