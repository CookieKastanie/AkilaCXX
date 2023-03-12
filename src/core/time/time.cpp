#include "akila/core/time/time.hpp"
#include "akila/core/math/math.hpp"

using namespace akila;
using namespace std::chrono_literals;

std::chrono::steady_clock Time::clock{};
std::chrono::steady_clock::time_point Time::realCurrentTime{};
std::chrono::steady_clock::time_point Time::frameCurrentTime{};
std::chrono::steady_clock::time_point Time::tickCurrentTime{};

std::chrono::nanoseconds Time::_deltaFrame{0ns};
std::chrono::nanoseconds Time::_deltaTick{0ns};
std::chrono::nanoseconds Time::accumulator{0ns};

float Time::timeScale = 1.f;

Time::TimeValues Time::presentedValues{};

void Time::setTickFrequency(float f) {
	_deltaTick = std::chrono::round<std::chrono::nanoseconds>(
		std::chrono::duration<float>(
			1.f / max(std::numeric_limits<float>::epsilon(), f)
		)
	);

	presentedValues.deltaTick = std::chrono::duration<float>(_deltaTick).count();
}

void Time::setTimeScale(float scale) {
	timeScale = scale;
}

void Time::setTimeScaleAndFrequency(float scale, float f) {
	timeScale = scale;
	setTickFrequency(f / scale);
}

void Time::init() {
	accumulator = 0ns;
	_deltaFrame = 0ns;
	_deltaTick = 0ns;

	realCurrentTime = clock.now();
	frameCurrentTime = std::chrono::steady_clock::time_point{};
	tickCurrentTime = std::chrono::steady_clock::time_point{};

	timeScale = 1.f;

	presentedValues.nowFrame = 0.f;
	presentedValues.nowTick = 0.f;
	presentedValues.contextNow = 0.f;
	setTickFrequency(64.f); // presentedValues.deltaTick
	presentedValues.deltaFrame = 0.f;
	presentedValues.contextDelta = 0.f;
	presentedValues.mix = 0.f;
	
	update();
}

void Time::update() {
	std::chrono::time_point const realNow = clock.now();
	std::chrono::duration const realDeltaTime = realNow - realCurrentTime;
	realCurrentTime = realNow;

	std::chrono::time_point const newCurrentTime = (
		frameCurrentTime + std::chrono::duration_cast<std::chrono::nanoseconds>(realDeltaTime * timeScale)
	);

	_deltaFrame = newCurrentTime - frameCurrentTime;
	frameCurrentTime = newCurrentTime;
	accumulator += std::chrono::duration_cast<std::chrono::nanoseconds>(_deltaFrame);

	presentedValues.nowFrame = std::chrono::duration<float>(frameCurrentTime.time_since_epoch()).count();
	presentedValues.deltaFrame = std::chrono::duration<float>(_deltaFrame).count();
}

unsigned int Time::tickCountThisFrame() {
	return static_cast<unsigned int>((float)accumulator.count() / _deltaTick.count());
}

bool Time::tickTimeRemaining() {
	return accumulator >= _deltaTick;
}

void Time::beforeTick() {
	accumulator -= _deltaTick;
	tickCurrentTime += _deltaTick;

	presentedValues.nowTick = std::chrono::duration<float>(tickCurrentTime.time_since_epoch()).count();
	presentedValues.contextNow = presentedValues.nowTick;
}

void Time::calculateMix() {
	presentedValues.mix = (float)accumulator.count() / _deltaTick.count();
}

void Time::beforeTicksLoop() {
	presentedValues.contextDelta = presentedValues.deltaTick;
}

void Time::beforeFrame() {
	presentedValues.contextDelta = presentedValues.deltaFrame;
	presentedValues.contextNow = presentedValues.nowFrame;
}
