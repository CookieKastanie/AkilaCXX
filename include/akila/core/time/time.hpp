#pragma once

#include <chrono>

namespace akila {
	class Time {
	public:
		static float now() { return presentedValues.contextNow; }
		static float nowFrame() { return presentedValues.nowFrame; }
		static float nowTick() { return presentedValues.nowTick; }
		static float delta() { return presentedValues.contextDelta; }
		static float deltaFrame() { return presentedValues.deltaFrame; }
		static float deltaTick() { return presentedValues.deltaTick; }
		static float mix() { return presentedValues.mix; }

		static void setTickFrequency(float f);
		static void setTimeScale(float scale);
		static void setTimeScaleAndFrequency(float scale, float f);

	private:
		friend class Core;

		static std::chrono::steady_clock clock;
		static std::chrono::steady_clock::time_point realCurrentTime;
		static std::chrono::steady_clock::time_point frameCurrentTime;
		static std::chrono::steady_clock::time_point tickCurrentTime;

		static std::chrono::nanoseconds _deltaFrame;
		static std::chrono::nanoseconds _deltaTick;
		static std::chrono::nanoseconds accumulator;

		static float timeScale;
		
		static struct TimeValues {
			float nowTick = 0.f;
			float nowFrame = 0.f;
			float contextNow = 0.f;
			float deltaTick = 0.f;
			float deltaFrame = 0.f;
			float contextDelta = 0.f;
			float mix = 0.f;
		} presentedValues;

		static void init();
		static void update();
		static unsigned int tickCountThisFrame();
		static void beforeTicksLoop();
		static void beforeTick();
		static bool tickTimeRemaining();
		static void calculateMix();
		static void beforeFrame();
	};
}
