#include "akila/random/random.hpp"
#include <chrono>

using namespace akila;

std::mt19937 Random::engine;

void Random::init() {
	engine = std::mt19937{std::random_device{}()};
}

float Random::getFloat(float from, float to) {
	std::uniform_real_distribution<float> dist{from, to};
	return dist(engine);
}

int Random::getInt(int from, int to) {
	std::uniform_int_distribution<int> dist{from, to};
	return dist(engine);
}

Vec2 Random::getVec2(float from, float to) {
	return {getFloat(from, to), getFloat(from, to)};
}

Vec3 Random::getVec3(float from, float to) {
	return {getFloat(from, to), getFloat(from, to), getFloat(from, to)};
}

float Random::getAngle() {
	return getFloat(0, PI * 2.f);
}

std::uint64_t Random::getUid() {
	using namespace std::chrono;
	time_point now = time_point_cast<seconds>(system_clock::now());

	seconds value = now.time_since_epoch();
	std::uint64_t uid = value.count() << 32;

	std::uniform_int_distribution<std::uint32_t> dist{0, static_cast<std::uint32_t>(-1)};
	uid |= static_cast<std::uint64_t>(dist(engine));

	return uid;
}
