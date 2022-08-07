#include "akila/random/random.hpp"

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
